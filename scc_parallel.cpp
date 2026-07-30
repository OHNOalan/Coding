// 并行 Forward-Backward (FB / DCSC) SCC 算法
// 两套调度器实现 + 粒度截断(granularity cut-off) + 伪共享(false sharing)对照实验
//
// 编译（不带 TBB，只测自研 work-stealing 线程池）:
//   g++ -std=c++17 -O2 -pthread -o scc_parallel scc_parallel.cpp
// 编译（带 TBB，两套调度器都测）:
//   g++ -std=c++17 -O2 -pthread -DUSE_TBB -o scc_parallel scc_parallel.cpp -ltbb
//
// 运行: ./scc_parallel
//
// ============================== 设计要点 ==============================
// 1. O(N) 内存分配隐患的修复：
//    每次递归不再 new 一个大小为 N 的 vector<char>，而是用 thread_local
//    的 scratch 数组（每个线程一份，生命周期内只分配一次），并且只清理
//    "这次用过的下标"（用 verts 自己当清理列表），而不是 memset 整个数组。
//    单次调用的开销变成 O(|verts| + deg(verts))，与全图大小 N 无关。
//
// 2. Cache 伪共享 (False Sharing) 的处理：
//    - inSet/F/B/disc/low/onStack 这些"热"scratch 数组做成 thread_local，
//      物理上就是每个线程自己的一份内存，根本不可能跨线程共享同一条
//      cache line —— 这是从"避免"伪共享升级为"消除"伪共享的关键设计。
//    - comp[] 这个全局结果数组，本来会被多个线程并发写入不同但物理上
//      可能相邻的下标（因为 F\B / B\F / rest 这几个子集的顶点在内存里是
//      交错分布的，不是连续的），必然造成 cache line 来回颠簸。
//      解决办法：每个线程把 (顶点, 所属SCC代表) 累积到自己的 thread_local
//      结果缓冲区里，并行阶段完全不碰共享的 comp[]；等整棵递归树全部
//      算完（task_group::wait() 返回）之后，主线程做一次单线程的合并
//      (mergeResults)，把所有线程的结果缓冲区串行写进 comp[]。
//      这样并行阶段对共享内存"零写入"，伪共享无从谈起。
//    - 任务队列 (WorkStealingPool::PaddedDeque) 用 alignas(64) 隔开，
//      避免不同线程自己的队列头（mutex + deque 控制块）被塞进同一条
//      cache line。
//    - g.adj / g.radj 是只读共享数据，多核并发"读"同一条 cache line
//      在 MESI 协议下走的是 Shared 状态，不产生写竞争，所以不需要特殊
//      处理——伪共享只在"写"共享内存时才会发生，这点很容易和"读共享"
//      混为一谈。
//    - pending 计数器（task_group 的 join 计数）是唯一"故意"跨线程共享
//      并发写的变量，这是真竞争（true sharing）不是伪共享，规模有限
//      （只在超过粒度阈值时才 fetch_add/fetch_sub，次数是 O(任务数)
//      而不是 O(N)），可以接受。
//
// 3. 粒度截断 (Granularity Cut-off)：
//    当 verts.size() <= threshold 时，不再切分/派发新任务，直接对这一小块
//    跑一个"局部 Tarjan"（restricted 到这个子集），单线程秒杀，避免小任务
//    的调度开销 > 计算本身开销。
//
// 4. 两套调度器共用同一份 fbSolve<TG> 模板逻辑（策略与机制分离）：
//    tbb::task_group 和自研 CustomTaskGroup 都只需要提供 .run(f) 接口，
//    FB 算法本体完全不关心底下是谁在调度。
// =======================================================================

#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>

#ifdef USE_TBB
#include <tbb/task_group.h>
#endif

using namespace std;

// ---------------------------------------------------------------
// Graph
// ---------------------------------------------------------------
struct Graph {
    int n;
    vector<vector<int>> adj, radj;
    Graph(int n = 0) : n(n), adj(n), radj(n) {}
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }
};

// ---------------------------------------------------------------
// Ground truth: 串行 Kosaraju（正确性已经在 scc_compare.cpp 里验证过）
// ---------------------------------------------------------------
vector<int> kosaraju(const Graph &g) {
    int n = g.n;
    vector<bool> visited(n, false);
    vector<int> order;
    order.reserve(n);
    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;
        vector<pair<int,int>> stk;
        visited[i] = true;
        stk.push_back({i, 0});
        while (!stk.empty()) {
            int u = stk.back().first;
            int idx = stk.back().second;
            if (idx < (int)g.adj[u].size()) {
                stk.back().second++;
                int v = g.adj[u][idx];
                if (!visited[v]) { visited[v] = true; stk.push_back({v, 0}); }
            } else {
                order.push_back(u);
                stk.pop_back();
            }
        }
    }
    vector<int> comp(n, -1);
    int c = 0;
    for (int i = n - 1; i >= 0; i--) {
        int s = order[i];
        if (comp[s] != -1) continue;
        vector<int> stk = {s};
        comp[s] = c;
        while (!stk.empty()) {
            int u = stk.back(); stk.pop_back();
            for (int v : g.radj[u]) if (comp[v] == -1) { comp[v] = c; stk.push_back(v); }
        }
        c++;
    }
    return comp;
}

vector<vector<int>> canonical(const vector<int> &comp) {
    map<int, vector<int>> groups;
    for (int i = 0; i < (int)comp.size(); i++) groups[comp[i]].push_back(i);
    vector<vector<int>> result;
    for (auto &[k, v] : groups) { sort(v.begin(), v.end()); result.push_back(v); }
    sort(result.begin(), result.end());
    return result;
}

// ---------------------------------------------------------------
// 自研 work-stealing 线程池
// 每个线程一条自己的双端队列：自己 push/pop 走 LIFO（局部性好），
// 别的线程来"偷"的时候走 FIFO（偷老任务，减少和 owner 抢新任务的冲突）。
// 用互斥锁保护每条队列——不是 lock-free 的 Chase-Lev deque，工程上更简单
// 更不容易写错；配合粒度截断后任务数量本来就不多，锁竞争可以接受。
// ---------------------------------------------------------------
class WorkStealingPool {
public:
    explicit WorkStealingPool(int numThreads) : deques(numThreads) {
        for (int i = 0; i < numThreads; i++) deques[i] = make_unique<PaddedDeque>();
        for (int i = 0; i < numThreads; i++) workers.emplace_back([this, i] { workerLoop(i); });
    }

    ~WorkStealingPool() {
        stopFlag.store(true, memory_order_release);
        cv.notify_all();
        for (auto &t : workers) if (t.joinable()) t.join();
    }

    void push(function<void()> f) {
        int idx = tls_myIndex;
        if (idx < 0 || idx >= (int)deques.size()) idx = 0; // 外部线程(如 main)提交到 0 号队列
        {
            lock_guard<mutex> lock(deques[idx]->mtx);
            deques[idx]->tasks.push_back(std::move(f));
        }
        cv.notify_all();
    }

    // 供 TaskGroup::wait() 调用：帮着干一件事而不是傻等，避免死等/利用率低
    bool helpOnce() {
        int idx = tls_myIndex;
        if (idx < 0 || idx >= (int)deques.size()) idx = 0;
        function<void()> task;
        if (popOrSteal(idx, task)) { task(); return true; }
        return false;
    }

private:
    struct alignas(64) PaddedDeque { // 对齐到 cache line，避免不同线程的队列头互相伪共享
        mutex mtx;
        deque<function<void()>> tasks;
    };

    bool popOrSteal(int selfIdx, function<void()> &out) {
        {
            auto &dq = *deques[selfIdx];
            lock_guard<mutex> lock(dq.mtx);
            if (!dq.tasks.empty()) {
                out = std::move(dq.tasks.back()); // 自己：LIFO
                dq.tasks.pop_back();
                return true;
            }
        }
        static thread_local mt19937 rng(random_device{}());
        int n = (int)deques.size();
        int start = n > 0 ? (int)(rng() % n) : 0;
        for (int k = 0; k < n; k++) {
            int victim = (start + k) % n;
            if (victim == selfIdx) continue;
            auto &dq = *deques[victim];
            lock_guard<mutex> lock(dq.mtx);
            if (!dq.tasks.empty()) {
                out = std::move(dq.tasks.front()); // 偷：FIFO，拿最老的任务
                dq.tasks.pop_front();
                return true;
            }
        }
        return false;
    }

    void workerLoop(int idx) {
        tls_myIndex = idx;
        while (!stopFlag.load(memory_order_acquire)) {
            function<void()> task;
            if (popOrSteal(idx, task)) {
                task();
            } else {
                unique_lock<mutex> lk(cvMtx);
                cv.wait_for(lk, chrono::microseconds(200),
                            [this] { return stopFlag.load(memory_order_acquire); });
            }
        }
    }

    vector<unique_ptr<PaddedDeque>> deques;
    vector<thread> workers;
    atomic<bool> stopFlag{false};
    mutex cvMtx;
    condition_variable cv;
    static thread_local int tls_myIndex;
};
thread_local int WorkStealingPool::tls_myIndex = -1;

// fork/join 语义封装：run() 派发，wait() 一次性等到整棵任务树算完。
// 不需要每层递归各建一个 TaskGroup —— 整个计算共用一个，最后只 wait 一次。
class CustomTaskGroup {
public:
    explicit CustomTaskGroup(WorkStealingPool &p) : pool(p) {}

    template <class F>
    void run(F f) {
        pending.fetch_add(1, memory_order_relaxed);
        pool.push([this, f = std::move(f)]() mutable {
            f();
            pending.fetch_sub(1, memory_order_acq_rel);
        });
    }

    void wait() {
        while (pending.load(memory_order_acquire) > 0) {
            if (!pool.helpOnce()) this_thread::yield();
        }
    }

private:
    WorkStealingPool &pool;
    atomic<int> pending{0};
};

// ---------------------------------------------------------------
// thread_local scratch + 跨线程结果合并机制
// ---------------------------------------------------------------
namespace scc_detail {

thread_local vector<char> tls_inSet, tls_F, tls_B, tls_onStack;
thread_local vector<int> tls_disc, tls_low;
thread_local mt19937 tls_rng(std::random_device{}());

inline void ensureScratch(int n) {
    if ((int)tls_inSet.size() != n) {
        tls_inSet.assign(n, 0);
        tls_F.assign(n, 0);
        tls_B.assign(n, 0);
        tls_onStack.assign(n, 0);
        tls_disc.assign(n, -1);
        tls_low.assign(n, -1);
    }
}

static atomic<long long> g_generation{0};
static mutex g_registryMtx;
static vector<vector<pair<int,int>>*> g_registry;

thread_local long long tls_generation = -1;
thread_local vector<pair<int,int>> tls_results;

// 每次跑一个新图之前调用：让所有线程（包括被复用的 main 线程）
// 在下次 emitResult 时都强制重新注册 + 清空自己的结果缓冲区，
// 避免跨图残留脏数据，也避免访问已销毁线程的悬空指针。
inline void beginNewRun() {
    g_generation.fetch_add(1, memory_order_relaxed);
    lock_guard<mutex> lock(g_registryMtx);
    g_registry.clear();
}

inline void ensureParticipation() {
    long long gen = g_generation.load(memory_order_relaxed);
    if (tls_generation != gen) {
        tls_generation = gen;
        tls_results.clear();
        lock_guard<mutex> lock(g_registryMtx);
        g_registry.push_back(&tls_results);
    }
}

inline void emitResult(int v, int rep) {
    ensureParticipation();
    tls_results.push_back({v, rep});
}

inline vector<int> mergeResults(int n) {
    // 这里跑的时候所有 worker 都已经 quiesce（wait() 已返回），
    // 单线程串行合并，彻底不存在并发写 comp[] 的问题。
    vector<int> comp(n, -1);
    lock_guard<mutex> lock(g_registryMtx);
    for (auto *buf : g_registry)
        for (auto &pr : *buf)
            comp[pr.first] = pr.second;
    return comp;
}

} // namespace scc_detail

// ---------------------------------------------------------------
// 粒度截断的"叶子"处理：局部 Tarjan，restrict 到 verts 这个子集
// ---------------------------------------------------------------
void serialTarjanSubset(const Graph &g, const vector<int> &verts) {
    using namespace scc_detail;
    if (verts.empty()) return;
    ensureScratch(g.n);
    for (int v : verts) tls_inSet[v] = 1;

    int timer = 0;
    vector<int> stk; // Tarjan 的"当前未封闭 SCC 候选栈"
    stk.reserve(verts.size());

    for (int root : verts) {
        if (tls_disc[root] != -1) continue;
        vector<pair<int,int>> callStk; // 迭代模拟递归: {node, 下一条边下标}
        tls_disc[root] = tls_low[root] = timer++;
        stk.push_back(root);
        tls_onStack[root] = 1;
        callStk.push_back({root, 0});

        while (!callStk.empty()) {
            int u = callStk.back().first;
            int idx = callStk.back().second;
            if (idx < (int)g.adj[u].size()) {
                callStk.back().second++;
                int v = g.adj[u][idx];
                if (!tls_inSet[v]) continue; // 限制在子集内
                if (tls_disc[v] == -1) {
                    tls_disc[v] = tls_low[v] = timer++;
                    stk.push_back(v);
                    tls_onStack[v] = 1;
                    callStk.push_back({v, 0});
                } else if (tls_onStack[v]) {
                    tls_low[u] = min(tls_low[u], tls_disc[v]);
                }
            } else {
                callStk.pop_back();
                if (!callStk.empty()) {
                    int parent = callStk.back().first;
                    tls_low[parent] = min(tls_low[parent], tls_low[u]);
                }
                if (tls_low[u] == tls_disc[u]) {
                    while (true) {
                        int v = stk.back(); stk.pop_back();
                        tls_onStack[v] = 0;
                        emitResult(v, u); // 代表元 = 封闭这个 SCC 的 Tarjan root u
                        if (v == u) break;
                    }
                }
            }
        }
    }

    for (int v : verts) { // 局部清理，O(|verts|) 而不是 O(n)
        tls_inSet[v] = 0; tls_disc[v] = -1; tls_low[v] = -1; tls_onStack[v] = 0;
    }
}

// ---------------------------------------------------------------
// 并行 Forward-Backward 主体：模板化，TG 只需要提供 .run(callable)
// ---------------------------------------------------------------
template <class TG>
void fbSolve(const Graph &g, vector<int> verts, TG &tg, int threshold) {
    using namespace scc_detail;
    if (verts.empty()) return;
    if ((int)verts.size() <= threshold) {
        serialTarjanSubset(g, verts);
        return;
    }

    ensureScratch(g.n);
    for (int v : verts) tls_inSet[v] = 1;

    int pivot = verts[tls_rng() % verts.size()]; // 随机 pivot，避免链式图的最坏切分

    { // 正向可达 F
        vector<int> stk = {pivot};
        tls_F[pivot] = 1;
        while (!stk.empty()) {
            int u = stk.back(); stk.pop_back();
            for (int v : g.adj[u])
                if (tls_inSet[v] && !tls_F[v]) { tls_F[v] = 1; stk.push_back(v); }
        }
    }
    { // 反向可达 B（在反图上的正向可达）
        vector<int> stk = {pivot};
        tls_B[pivot] = 1;
        while (!stk.empty()) {
            int u = stk.back(); stk.pop_back();
            for (int v : g.radj[u])
                if (tls_inSet[v] && !tls_B[v]) { tls_B[v] = 1; stk.push_back(v); }
        }
    }

    vector<int> onlyF, onlyB, rest;
    onlyF.reserve(verts.size()); onlyB.reserve(verts.size()); rest.reserve(verts.size());
    for (int v : verts) {
        bool f = tls_F[v], b = tls_B[v];
        if (f && b) emitResult(v, pivot);
        else if (f) onlyF.push_back(v);
        else if (b) onlyB.push_back(v);
        else rest.push_back(v);
        tls_inSet[v] = 0; tls_F[v] = 0; tls_B[v] = 0; // 局部清理
    }

    // 三块全部 tg.run() 派发，不做"内联执行最后一块"的优化：
    // 那种优化只有在能确定被内联那块的规模是有界的时候才安全。FB 算法里
    // rest 到底大不大完全取决于图的拓扑——比如很多个互不相连的独立分量时，
    // rest 反而是最大的一块。内联执行会让这块工作对 work-stealing 调度器
    // 整个不可见（它只是当前线程调用栈上的一次普通嵌套调用，不在任何队列
    // 里），其它空闲线程根本偷不到，等于把最大的一块活硬压成串行。
    //
    // 用 shared_ptr 包一层而不是直接 init-capture + std::move：
    // tbb::task_group::run() 内部有的路径会通过 const 引用调用存下来的
    // 可调用对象，这要求 operator() 本身是 const 的；而"从捕获变量里
    // move 出去"必须把 lambda 标成 mutable，operator() 就不再是 const 了，
    // 在 TBB 下会编译失败 (自研的 WorkStealingPool 因为用 std::function
    // 存储、按值调用，不受影响，但两套调度器要共用同一份 fbSolve 代码，
    // 就必须迁就更严格的那一个)。shared_ptr 本身按值捕获（只是拷贝一次
    // 引用计数，不拷贝底层 vector），在 lambda 里解引用后 move 出来，
    // 不需要 mutable，两边都能编译，还是零额外的 vector 拷贝。
    auto onlyFPtr = make_shared<vector<int>>(std::move(onlyF));
    tg.run([&g, &tg, threshold, onlyFPtr]() {
        fbSolve(g, std::move(*onlyFPtr), tg, threshold);
    });
    auto onlyBPtr = make_shared<vector<int>>(std::move(onlyB));
    tg.run([&g, &tg, threshold, onlyBPtr]() {
        fbSolve(g, std::move(*onlyBPtr), tg, threshold);
    });
    auto restPtr = make_shared<vector<int>>(std::move(rest));
    tg.run([&g, &tg, threshold, restPtr]() {
        fbSolve(g, std::move(*restPtr), tg, threshold);
    });
}

// 接受一个已经建好的线程池（可以跨多次调用复用，避免每次都重新创建/
// 销毁 N 条 OS 线程——那个开销在小规模基准测试里会掩盖掉真实的并行收益）
vector<int> forwardBackwardParallel_WS(WorkStealingPool &pool, const Graph &g, int threshold) {
    scc_detail::beginNewRun();
    CustomTaskGroup tg(pool);
    vector<int> all(g.n);
    iota(all.begin(), all.end(), 0);
    tg.run([&g, &tg, threshold, all]() { fbSolve(g, all, tg, threshold); }); // 顶层只调用一次，多一次拷贝可忽略
    tg.wait();
    return scc_detail::mergeResults(g.n);
}

// 便利重载：自己建一次性的线程池（用于正确性压测这种一次性调用场景）
vector<int> forwardBackwardParallel_WS(const Graph &g, int numThreads, int threshold) {
    WorkStealingPool pool(numThreads);
    return forwardBackwardParallel_WS(pool, g, threshold);
}

// 诊断用: 零调度开销的"假并行"——run() 就是直接同步调用，没有 std::function、
// 没有锁、没有线程。用来把 "FB 算法本身的计算量" 和 "调度器机制的开销"
// 这两件事分开测量，看差距到底来自哪一边。
struct InlineTaskGroup {
    template <class F> void run(F f) { f(); }
    void wait() {}
};

vector<int> forwardBackwardSerialDirect(const Graph &g, int threshold) {
    scc_detail::beginNewRun();
    InlineTaskGroup tg;
    vector<int> all(g.n);
    iota(all.begin(), all.end(), 0);
    fbSolve(g, std::move(all), tg, threshold);
    return scc_detail::mergeResults(g.n);
}

#ifdef USE_TBB
vector<int> forwardBackwardParallel_TBB(const Graph &g, int threshold) {
    scc_detail::beginNewRun();
    tbb::task_group tg;
    vector<int> all(g.n);
    iota(all.begin(), all.end(), 0);
    tg.run([&g, &tg, threshold, all]() { fbSolve(g, all, tg, threshold); }); // 顶层只调用一次，多一次拷贝可忽略
    tg.wait();
    return scc_detail::mergeResults(g.n);
}
#endif

// ---------------------------------------------------------------
// "反面教材"版本：用全局共享数组（不是 thread_local），直接写共享 comp[]。
// 逻辑上仍然正确（不同任务碰的下标集合不相交，没有真正的 data race），
// 但这些下标在内存里是交错分布的，必然引起 cache line 在核间来回颠簸——
// 用来和上面的 thread_local 版本做伪共享的 A/B 对照实验。
// 仅用于教学/benchmark，不建议在生产代码里这样写。
// ---------------------------------------------------------------
namespace naive_shared {
vector<char> g_inSet, g_F, g_B;
vector<int> g_comp;
mutex g_allocMtx;

void ensure(int n) {
    lock_guard<mutex> lock(g_allocMtx);
    if ((int)g_inSet.size() != n) {
        g_inSet.assign(n, 0);
        g_F.assign(n, 0);
        g_B.assign(n, 0);
        g_comp.assign(n, -1);
    }
}
}

template <class TG>
void fbSolveNaiveShared(const Graph &g, vector<int> verts, TG &tg, int threshold) {
    using namespace naive_shared;
    if (verts.empty()) return;
    if ((int)verts.size() <= threshold) {
        // 简化起见，截断后仍用共享数组直接完成（保持同一份"共享内存"风格，
        // 专注对比"并发写共享数组"这一件事本身）
        for (int v : verts) g_inSet[v] = 1;
        // 局部 BFS 从任意点出发做一次简单可达性求 SCC（够用即可，不追求最优）
        // 这里直接退化为对每个未分配的点各自跑一次 forward/backward，
        // 仅用于制造和 thread_local 版本相同数量级的共享内存写入。
        for (int root : verts) {
            if (g_comp[root] != -1) continue;
            vector<char> F(0), B(0); // 占位，实际逻辑与下方一致但不再展开
            vector<int> stk = {root};
            vector<char> f(g.n, 0), b(g.n, 0);
            f[root] = 1;
            while (!stk.empty()) { int u = stk.back(); stk.pop_back();
                for (int v : g.adj[u]) if (g_inSet[v] && !f[v]) { f[v]=1; stk.push_back(v);} }
            stk = {root}; b[root] = 1;
            while (!stk.empty()) { int u = stk.back(); stk.pop_back();
                for (int v : g.radj[u]) if (g_inSet[v] && !b[v]) { b[v]=1; stk.push_back(v);} }
            for (int v : verts) if (f[v] && b[v]) g_comp[v] = root; // 直接写共享 comp[]
        }
        for (int v : verts) g_inSet[v] = 0;
        return;
    }

    for (int v : verts) g_inSet[v] = 1; // 并发写共享数组：故意制造伪共享
    int pivot = verts[rand() % verts.size()];

    { vector<int> stk = {pivot}; g_F[pivot] = 1;
      while (!stk.empty()) { int u = stk.back(); stk.pop_back();
          for (int v : g.adj[u]) if (g_inSet[v] && !g_F[v]) { g_F[v]=1; stk.push_back(v);} } }
    { vector<int> stk = {pivot}; g_B[pivot] = 1;
      while (!stk.empty()) { int u = stk.back(); stk.pop_back();
          for (int v : g.radj[u]) if (g_inSet[v] && !g_B[v]) { g_B[v]=1; stk.push_back(v);} } }

    vector<int> onlyF, onlyB, rest;
    for (int v : verts) {
        bool f = g_F[v], b = g_B[v];
        if (f && b) g_comp[v] = pivot; // 并发写共享 comp[]，制造伪共享
        else if (f) onlyF.push_back(v);
        else if (b) onlyB.push_back(v);
        else rest.push_back(v);
        g_inSet[v] = 0; g_F[v] = 0; g_B[v] = 0; // 并发写共享数组，制造伪共享
    }

    auto onlyFPtr = make_shared<vector<int>>(std::move(onlyF));
    tg.run([&g, &tg, threshold, onlyFPtr]() {
        fbSolveNaiveShared(g, std::move(*onlyFPtr), tg, threshold);
    });
    auto onlyBPtr = make_shared<vector<int>>(std::move(onlyB));
    tg.run([&g, &tg, threshold, onlyBPtr]() {
        fbSolveNaiveShared(g, std::move(*onlyBPtr), tg, threshold);
    });
    auto restPtr = make_shared<vector<int>>(std::move(rest));
    tg.run([&g, &tg, threshold, restPtr]() {
        fbSolveNaiveShared(g, std::move(*restPtr), tg, threshold);
    });
}

vector<int> forwardBackwardParallel_WS_naiveShared(WorkStealingPool &pool, const Graph &g, int threshold) {
    naive_shared::ensure(g.n);
    CustomTaskGroup tg(pool);
    vector<int> all(g.n);
    iota(all.begin(), all.end(), 0);
    tg.run([&g, &tg, threshold, all]() { fbSolveNaiveShared(g, all, tg, threshold); });
    tg.wait();
    return naive_shared::g_comp;
}

vector<int> forwardBackwardParallel_WS_naiveShared(const Graph &g, int numThreads, int threshold) {
    WorkStealingPool pool(numThreads);
    return forwardBackwardParallel_WS_naiveShared(pool, g, threshold);
}

// ---------------------------------------------------------------
// 生成一条"SCC 链"合成图: k 个大小为 m 的块，每块内部强连通(一个 SCC)，
// 块之间只有前向边 (block i -> block i+1)，保证条件图是一条链。
// 这样递归会有真实的深度和均衡的并行子任务，适合压测调度器。
// ---------------------------------------------------------------
Graph makeChainOfSCCs(int k, int m, int extraIntraEdgesPerNode, int extraInterEdges, mt19937 &rng) {
    int n = k * m;
    Graph g(n);
    auto id = [&](int block, int idx) { return block * m + idx; };
    for (int b = 0; b < k; b++) {
        for (int i = 0; i < m; i++) g.addEdge(id(b, i), id(b, (i + 1) % m)); // 保证块内强连通
        for (int i = 0; i < m; i++)
            for (int e = 0; e < extraIntraEdgesPerNode; e++) {
                int j = rng() % m;
                if (j != i) g.addEdge(id(b, i), id(b, j));
            }
    }
    for (int b = 0; b + 1 < k; b++)
        for (int e = 0; e < extraInterEdges; e++)
            g.addEdge(id(b, rng() % m), id(b + 1, rng() % m)); // 只往后连，保证是链
    return g;
}

// ---------------------------------------------------------------
// 生成"互相独立的多组链"：numGroups 组之间零边相连，每组内部是一条
// 长度为 k、块大小为 m 的 SCC 链。目的：让第一层 pivot 一选下去，
// F∩B 之外的 rest 立刻就是"其它 numGroups-1 组的全部顶点"——一个天然
// 巨大且完全独立、可以真正并行展开的子问题，而不是像单条链那样，
// 靠前的几层递归本身就带有强制的前后依赖关系、可用并行度有限。
// ---------------------------------------------------------------
Graph makeForestOfChains(int numGroups, int k, int m, mt19937 &rng) {
    int groupSize = k * m;
    int n = numGroups * groupSize;
    Graph g(n);
    auto id = [&](int grp, int block, int idx) { return grp * groupSize + block * m + idx; };
    for (int grp = 0; grp < numGroups; grp++) {
        for (int b = 0; b < k; b++) {
            for (int i = 0; i < m; i++) g.addEdge(id(grp, b, i), id(grp, b, (i + 1) % m));
            for (int i = 0; i < m; i++) {
                int j = rng() % m;
                if (j != i) g.addEdge(id(grp, b, i), id(grp, b, j));
            }
        }
        for (int b = 0; b + 1 < k; b++)
            for (int e = 0; e < 3; e++)
                g.addEdge(id(grp, b, rng() % m), id(grp, b + 1, rng() % m));
    }
    return g;
}

// ---------------------------------------------------------------
// main: 正确性压测 + 性能对比
// ---------------------------------------------------------------
int main() {
    mt19937 rng(20260729);
    int hwThreads = (int)thread::hardware_concurrency();
    if (hwThreads <= 0) hwThreads = 4;
    cout << "hardware_concurrency = " << hwThreads << "\n\n";

    // ============ 正确性压测（小随机图） ============
    {
        int trials = 300, failures = 0;
        for (int t = 0; t < trials; t++) {
            int n = 1 + (int)(rng() % 40);
            Graph g(n);
            int maxEdges = n * n;
            int edgeAttempts = rng() % (maxEdges + 1);
            for (int e = 0; e < edgeAttempts; e++) {
                int u = rng() % n, v = rng() % n;
                if (u != v) g.addEdge(u, v);
            }
            auto truth = canonical(kosaraju(g));

            int threads = 1 + (int)(rng() % hwThreads);
            int threshold = 1 + (int)(rng() % 5);
            auto ws = canonical(forwardBackwardParallel_WS(g, threads, threshold));
            if (ws != truth) { cerr << "WS mismatch at trial " << t << "\n"; failures++; }

#ifdef USE_TBB
            auto tb = canonical(forwardBackwardParallel_TBB(g, threshold));
            if (tb != truth) { cerr << "TBB mismatch at trial " << t << "\n"; failures++; }
#endif

            auto naive = canonical(forwardBackwardParallel_WS_naiveShared(g, threads, threshold));
            if (naive != truth) { cerr << "naive-shared mismatch at trial " << t << "\n"; failures++; }
        }
        if (failures == 0)
            cout << "[PASS] " << trials << " 组随机图，WS 线程池"
#ifdef USE_TBB
                 << " / TBB"
#endif
                 << " / naive-shared 三个并行实现全部和 Kosaraju 结果一致\n\n";
        else {
            cout << "[FAIL] " << failures << " 处不一致，见上面 stderr\n";
            return 1;
        }
    }

    // ============ 性能基准 ============
    auto timeit = [](function<void()> f) {
        auto t0 = chrono::steady_clock::now();
        f();
        auto t1 = chrono::steady_clock::now();
        return chrono::duration<double, milli>(t1 - t0).count();
    };
    auto best = [&](function<void()> f, int repeats = 3) {
        double m = 1e18;
        for (int i = 0; i < repeats; i++) m = min(m, timeit(f));
        return m;
    };

    int threshold = 500;
    auto runBenchmarkSuite = [&](const Graph &g, const string &label) {
        cout << "\n==== " << label << " (n=" << g.n << ") ====\n";
        vector<int> refComp;
        double tSerial = best([&] { refComp = kosaraju(g); });
        cout << "serial Kosaraju:        " << tSerial << " ms\n";

        for (int threads : {1, 2, 4, hwThreads}) {
            // 池在这里建一次，跨 repeats 复用——每次调用都重新拉起/销毁
            // N 条 OS 线程的开销，在这种规模的图上会掩盖掉真实的并行收益
            WorkStealingPool pool(threads);
            vector<int> comp;
            double t = best([&] { comp = forwardBackwardParallel_WS(pool, g, threshold); });
            bool ok = canonical(comp) == canonical(refComp);
            cout << "WS pool  threads=" << setw(2) << threads << "  threshold=" << threshold
                 << "  time=" << t << " ms  " << (ok ? "[correct]" : "[WRONG]") << "\n";
        }

#ifdef USE_TBB
        for (int threads : {1, 2, 4, hwThreads}) {
            // tbb::task_group 本身的并行度由全局 tbb::global_control 控制；
            // 这里只演示正确性和 threshold 效果，线程数限制留给
            // Docker 里 --cpus 或 oneTBB 的 global_control API 去做
            vector<int> comp;
            double t = best([&] { comp = forwardBackwardParallel_TBB(g, threshold); });
            bool ok = canonical(comp) == canonical(refComp);
            cout << "TBB task_group  threshold=" << threshold
                 << "  time=" << t << " ms  " << (ok ? "[correct]" : "[WRONG]") << "\n";
        }
#endif

        cout << "---- naive-shared-array 版本 (伪共享 A/B 对照) ----\n";
        for (int threads : {1, 2, 4, hwThreads}) {
            WorkStealingPool pool(threads);
            vector<int> comp;
            double t = best([&] { comp = forwardBackwardParallel_WS_naiveShared(pool, g, threshold); });
            bool ok = canonical(comp) == canonical(refComp);
            cout << "naive-shared threads=" << setw(2) << threads << "  threshold=" << threshold
                 << "  time=" << t << " ms  " << (ok ? "[correct]" : "[WRONG]") << "\n";
        }
    };

    // 图放大到 serial baseline ~几百 ms 的量级，这样线程池自身的固定开销
    // （建线程、steal 用的锁）占比才不会掩盖掉真实的并行收益信号
    // 图 1: 单条长链 —— 前几层递归自带强制的前后依赖，可用并行度受限
    Graph chain = makeChainOfSCCs(256, 8000, 2, 3, rng);
    runBenchmarkSuite(chain, "单条 SCC 链 (256 blocks x 8000)");

    // 图 2: 多组互相独立的链 —— 第一层切分后 rest 立刻是一个巨大且完全
    // 独立的子问题，天然适合展开成多个可并行的任务
    Graph forest = makeForestOfChains(hwThreads * 4, 16, 2000, rng);
    runBenchmarkSuite(forest, "互相独立的多组链 (" + to_string(hwThreads * 4) + " groups x 16 blocks x 2000)");

    // ============ 诊断实验: threshold 扫描 ============
    // 怀疑 WS 池不 scale 是因为任务粒度太细：每个任务的固定成本
    // (std::function 堆分配 + push/steal 抢锁) 超过了它自己的计算量。
    // 用同一张图、同一个线程数，只改 threshold，看看调大粒度是否能显著改善。
    cout << "\n==== 诊断: threshold 扫描 (forest 图, threads=" << hwThreads << ") ====\n";
    {
        vector<int> refComp = kosaraju(forest);
        WorkStealingPool sharedPool(hwThreads); // 只建一次，摊掉每个线程 O(n) scratch 首触发分配的成本
        for (int th : {200, 2000, 20000, 200000}) {
            vector<int> comp;
            double t = best([&] { comp = forwardBackwardParallel_WS(sharedPool, forest, th); }, 3);
            bool ok = canonical(comp) == canonical(refComp);
            cout << "threshold=" << setw(7) << th << "  time=" << t << " ms  "
                 << (ok ? "[correct]" : "[WRONG]") << "\n";
        }

        cout << "\n---- 零调度开销对照: InlineTaskGroup (纯 FB 算法本身的成本) ----\n";
        for (int th : {200, 2000, 20000, 200000}) {
            vector<int> comp;
            double t = best([&] { comp = forwardBackwardSerialDirect(forest, th); }, 3);
            bool ok = canonical(comp) == canonical(refComp);
            cout << "threshold=" << setw(7) << th << "  time=" << t << " ms  "
                 << (ok ? "[correct]" : "[WRONG]") << "\n";
        }
    }

    return 0;
}

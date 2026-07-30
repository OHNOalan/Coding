// SCC 三种算法对比测试：Kosaraju / Tarjan / Forward-Backward (FB)
// 编译: g++ -std=c++17 -O2 -Wall -o scc_compare scc_compare.cpp
// 运行: ./scc_compare
//
// ============================= 结论 =============================
// Kosaraju 第一遍必须是 DFS，不能换成 BFS；第二遍随便换（可以是 BFS）。
//
// 原因：Kosaraju 第一遍要产生的不是"任意顺序"，而是缩点后 DAG 的一个
// 逆拓扑序：只要 SCC(u) -> SCC(v) 是不同 SCC 间的一条边，就必须有
// f(u) > f(v)。DFS 的 finish time 天然满足这一点，靠的是"嵌套/白色路径
// 性质"——DFS 走到未访问点 u 时，会把从 u 出发能到达的所有未访问点都
// 立刻递归进去，变成 u 的后代，且必须在 u 回溯之前全部 finish；所以只要
// A、B 是不同 SCC 且有边 A->B，谁先被发现，"先到者"就会把"后者"整个
// 吞进自己的子树、等它全 finish 完自己才 finish，故 finish(A) > finish(B)
// 恒成立，与发现顺序无关。
//
// BFS 没有"吞子树"这个性质：它按层扩展，碰到邻居只是入队，不会立刻钻
// 进去处理完整个可达集合再回来处理其它邻居。BFS 的出队顺序本质上约等于
// "发现顺序"（跟 start time 是同一类东西），会重现 start-time 版本的经典
// 反例：最简单的 A->B 图（0->1），BFS 出队序 [0,1]，倒着处理会先选到 1，
// 从 1 在反图上一步就摸到 0，把两个本该独立的 SCC 错误合并成一个。
// 见 kosaraju_brokenBFSFirstPass()，随机压力测试里约 27% (1358/5000) 的图
// 会因此出错，包括 A->B 这个最小反例。
//
// 第二遍不产生任何"顺序信息"，只是从已排好的种子点出发在反图上做一次
// 纯连通性扫荡 (flood fill)：谁能从种子到达就归到同一个 SCC，正确性只
// 取决于"最终能到达的点集合"，与扫描顺序无关。所以把栈换成队列
// （kosaraju_BFSSecondPass()）在 5000 组随机图里 0 出错，与原版逐点一致。
//
// 一句话：第一遍在乎"顺序"，靠 DFS 的嵌套性质撑住正确性；
//         第二遍在乎"连通集合"，顺序无关紧要，DFS/BFS 随便选。
// ==================================================================
#include <bits/stdc++.h>
using namespace std;

struct Graph {
    int n;
    vector<vector<int>> adj, radj; // radj = reverse graph
    Graph(int n) : n(n), adj(n), radj(n) {}
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
    }
};

// ---------------------------------------------------------------
// 1) Kosaraju: 第一遍 DFS 求 finish time 顺序，第二遍在反图上按
//    finish time 从大到小取未访问点做 DFS/BFS，每次能到达的就是一个 SCC。
// ---------------------------------------------------------------
vector<int> kosaraju(const Graph &g) {
    int n = g.n;
    vector<bool> visited(n, false);
    vector<int> order;
    order.reserve(n);

    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;
        vector<pair<int,int>> stk; // {node, next edge index}
        visited[i] = true;
        stk.push_back({i, 0});
        while (!stk.empty()) {
            int u = stk.back().first;
            int idx = stk.back().second;
            if (idx < (int)g.adj[u].size()) {
                stk.back().second++; // 先写回，避免 push_back 后引用失效
                int v = g.adj[u][idx];
                if (!visited[v]) {
                    visited[v] = true;
                    stk.push_back({v, 0});
                }
            } else {
                order.push_back(u); // 出栈即 finish
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
            for (int v : g.radj[u]) {
                if (comp[v] == -1) {
                    comp[v] = c;
                    stk.push_back(v);
                }
            }
        }
        c++;
    }
    return comp;
}

// ---------------------------------------------------------------
// 1b) 实验版: 第一遍改用 BFS 求 "finish order"（用出队顺序模拟 finish time）。
//     预期: 会 WRONG，因为 BFS 出队顺序不满足 DFS 的嵌套/白色路径性质——
//     它本质上退化成了"start time 类"顺序，和讨论里分析的反例是同一个坑。
// ---------------------------------------------------------------
vector<int> kosaraju_brokenBFSFirstPass(const Graph &g) {
    int n = g.n;
    vector<bool> visited(n, false);
    vector<int> order; // 用"出队顺序"冒充 finish time

    for (int i = 0; i < n; i++) {
        if (visited[i]) continue;
        queue<int> q;
        visited[i] = true;
        q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u); // <-- 关键: 这不是 finish time, 只是出队/发现顺序
            for (int v : g.adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
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
            for (int v : g.radj[u]) {
                if (comp[v] == -1) { comp[v] = c; stk.push_back(v); }
            }
        }
        c++;
    }
    return comp;
}

// ---------------------------------------------------------------
// 1c) 实验版: 第一遍保持正确 DFS(拿到真正的 finish time)，
//     但第二遍（反图上找 SCC 的部分）改用 BFS 队列而不是栈。
//     预期: 完全正确。因为第二遍只是"从种子点出发的可达性扫荡"，
//     谁先谁后被访问无所谓，只要最终能到达的点集合一样即可——
//     这一步根本不依赖任何遍历顺序的性质。
// ---------------------------------------------------------------
vector<int> kosaraju_BFSSecondPass(const Graph &g) {
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
        queue<int> q; // <-- 唯一区别: 用队列 (BFS) 代替栈 (DFS)
        q.push(s);
        comp[s] = c;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g.radj[u]) {
                if (comp[v] == -1) { comp[v] = c; q.push(v); }
            }
        }
        c++;
    }
    return comp;
}

// ---------------------------------------------------------------
// 2) Tarjan: 单遍 DFS，维护 disc/low，用栈显式维护"当前仍可能属于
//    未封闭 SCC 的点"；low[u]==disc[u] 时把栈里到 u 为止的点弹出成一个 SCC。
// ---------------------------------------------------------------
struct TarjanSCC {
    const Graph &g;
    vector<int> disc, low, comp;
    vector<bool> onStack;
    vector<int> stk;
    int timer = 0, compCount = 0;

    TarjanSCC(const Graph &g)
        : g(g), disc(g.n, -1), low(g.n, -1), comp(g.n, -1), onStack(g.n, false) {}

    // 迭代版 DFS，避免大图递归爆栈；逻辑与递归版 Tarjan 等价
    void dfs(int start) {
        vector<tuple<int,int>> callStk; // {node, next edge index}
        disc[start] = low[start] = timer++;
        stk.push_back(start);
        onStack[start] = true;
        callStk.push_back({start, 0});

        while (!callStk.empty()) {
            auto &[u, idx] = callStk.back();
            if (idx < (int)g.adj[u].size()) {
                int v = g.adj[u][idx];
                idx++;
                if (disc[v] == -1) {
                    disc[v] = low[v] = timer++;
                    stk.push_back(v);
                    onStack[v] = true;
                    callStk.push_back({v, 0});
                } else if (onStack[v]) {
                    low[u] = min(low[u], disc[v]);
                }
            } else {
                callStk.pop_back();
                if (!callStk.empty()) {
                    int parent = get<0>(callStk.back());
                    low[parent] = min(low[parent], low[u]);
                }
                if (low[u] == disc[u]) {
                    while (true) {
                        int v = stk.back(); stk.pop_back();
                        onStack[v] = false;
                        comp[v] = compCount;
                        if (v == u) break;
                    }
                    compCount++;
                }
            }
        }
    }

    vector<int> run() {
        for (int i = 0; i < g.n; i++)
            if (disc[i] == -1) dfs(i);
        return comp;
    }
};

// ---------------------------------------------------------------
// 3) Forward-Backward (FB / Sharir 无时间戳变种):
//    在当前顶点子集中任选 pivot v，
//    F = 子集内从 v 正向可达的点， B = 子集内能到达 v 的点（反图正向可达）。
//    F∩B 就是 v 所在的 SCC；剩余点分裂成 F\B、B\F、其余三块，互不相交，
//    分别递归即可 —— 不需要任何 finishing/start time。
// ---------------------------------------------------------------
vector<int> forwardBackward(const Graph &g) {
    int n = g.n;
    vector<int> comp(n, -1);
    int compCount = 0;

    function<void(vector<int>&)> solve = [&](vector<int> &verts) {
        if (verts.empty()) return;
        if ((int)verts.size() == 1) {
            comp[verts[0]] = compCount++;
            return;
        }

        vector<char> inSet(n, false);
        for (int v : verts) inSet[v] = true;
        int pivot = verts[0];

        vector<char> F(n, false), B(n, false);
        {
            vector<int> stk = {pivot};
            F[pivot] = true;
            while (!stk.empty()) {
                int u = stk.back(); stk.pop_back();
                for (int v : g.adj[u])
                    if (inSet[v] && !F[v]) { F[v] = true; stk.push_back(v); }
            }
        }
        {
            vector<int> stk = {pivot};
            B[pivot] = true;
            while (!stk.empty()) {
                int u = stk.back(); stk.pop_back();
                for (int v : g.radj[u])
                    if (inSet[v] && !B[v]) { B[v] = true; stk.push_back(v); }
            }
        }

        vector<int> onlyF, onlyB, rest, sccNodes;
        for (int v : verts) {
            bool f = F[v], b = B[v];
            if (f && b) sccNodes.push_back(v);
            else if (f) onlyF.push_back(v);
            else if (b) onlyB.push_back(v);
            else rest.push_back(v);
        }

        int myComp = compCount++;
        for (int v : sccNodes) comp[v] = myComp;

        solve(onlyF);
        solve(onlyB);
        solve(rest);
    };

    vector<int> all(n);
    iota(all.begin(), all.end(), 0);
    solve(all);
    return comp;
}

// ---------------------------------------------------------------
// 工具函数：把 comp[] 转成"排序后的点集合列表"，用于跨算法比较
// （不同算法给出的 component id 编号规则不同，必须靠内容比较）
// ---------------------------------------------------------------
vector<vector<int>> canonical(const vector<int> &comp) {
    map<int, vector<int>> groups;
    for (int i = 0; i < (int)comp.size(); i++) groups[comp[i]].push_back(i);
    vector<vector<int>> result;
    for (auto &[k, v] : groups) {
        sort(v.begin(), v.end());
        result.push_back(v);
    }
    sort(result.begin(), result.end());
    return result;
}

void printGraph(const Graph &g) {
    for (int u = 0; u < g.n; u++)
        for (int v : g.adj[u])
            cerr << u << " -> " << v << "\n";
}

void printSCCs(const vector<vector<int>> &c) {
    for (auto &comp : c) {
        cerr << "  { ";
        for (int v : comp) cerr << v << " ";
        cerr << "}\n";
    }
}

bool checkAllAgree(const Graph &g, bool verbose = false) {
    auto c1 = canonical(kosaraju(g));
    auto c2 = canonical(TarjanSCC(g).run());
    auto c3 = canonical(forwardBackward(g));
    bool ok = (c1 == c2) && (c2 == c3);
    if (!ok || verbose) {
        cerr << "---- graph (n=" << g.n << ") ----\n";
        printGraph(g);
        cerr << "Kosaraju:\n"; printSCCs(c1);
        cerr << "Tarjan:\n"; printSCCs(c2);
        cerr << "ForwardBackward:\n"; printSCCs(c3);
    }
    return ok;
}

int main() {
    // ---- 手工测试 1: 讨论里的反例图 A -> B ----
    {
        Graph g(2);
        g.addEdge(0, 1);
        bool ok = checkAllAgree(g);
        assert(ok);
        cout << "[PASS] A->B (2 nodes, expect 2 singleton SCCs)\n";
    }

    // ---- 手工测试 2: 经典链式 SCC 图 (CLRS 风格) ----
    // {0,1,2} -> {3,4,5} -> {6,7}
    {
        Graph g(8);
        g.addEdge(0, 1); g.addEdge(1, 2); g.addEdge(2, 0);   // SCC {0,1,2}
        g.addEdge(2, 3);
        g.addEdge(3, 4); g.addEdge(4, 5); g.addEdge(5, 3);   // SCC {3,4,5}
        g.addEdge(5, 6);
        g.addEdge(6, 7); g.addEdge(7, 6);                    // SCC {6,7}
        bool ok = checkAllAgree(g, true);
        assert(ok);
        cout << "[PASS] chained SCC {0,1,2} -> {3,4,5} -> {6,7}\n";
    }

    // ---- 手工测试 3: 单个大环 (all one SCC) ----
    {
        Graph g(6);
        for (int i = 0; i < 6; i++) g.addEdge(i, (i + 1) % 6);
        bool ok = checkAllAgree(g);
        assert(ok);
        cout << "[PASS] single 6-cycle (one big SCC)\n";
    }

    // ---- 手工测试 4: 完全没有边 (all singletons) ----
    {
        Graph g(5);
        bool ok = checkAllAgree(g);
        assert(ok);
        cout << "[PASS] no edges (5 singleton SCCs)\n";
    }

    // ---- 随机压力测试 ----
    mt19937 rng(20260729);
    int trials = 5000;
    int failures = 0;
    for (int t = 0; t < trials; t++) {
        int n = 1 + (int)(rng() % 10); // 1..10 个点
        Graph g(n);
        int maxEdges = n * n;
        int edgeAttempts = rng() % (maxEdges + 1);
        for (int e = 0; e < edgeAttempts; e++) {
            int u = rng() % n, v = rng() % n;
            if (u != v) g.addEdge(u, v); // 允许重边，不影响 SCC 结果
        }
        if (!checkAllAgree(g)) {
            cerr << "MISMATCH at trial " << t << "\n";
            failures++;
            if (failures > 5) break; // 别刷屏
        }
    }

    if (failures == 0) {
        cout << "[PASS] " << trials << " random stress tests, all three algorithms agree\n";
    } else {
        cout << "[FAIL] " << failures << " mismatches found, see stderr above\n";
        return 1;
    }

    // -----------------------------------------------------------
    // 实验: 第一遍换成 BFS 是否还正确？ 用最简单的 A->B 就先验证
    // -----------------------------------------------------------
    {
        Graph g(2);
        g.addEdge(0, 1);
        auto broken = canonical(kosaraju_brokenBFSFirstPass(g));
        auto correct = canonical(kosaraju(g));
        cout << "\n[experiment] first-pass-BFS Kosaraju on A->B:\n";
        cerr << "  broken(BFS-first-pass) result: "; printSCCs(broken);
        cerr << "  correct(DFS-first-pass) result: "; printSCCs(correct);
        if (broken != correct)
            cout << "  => 确认: BFS 第一遍把 {A},{B} 错误合并成了一个 SCC (预期内的失败)\n";
        else
            cout << "  => 意外: 这次没有失败？(不应该发生)\n";
    }

    int brokenFail = 0, brokenTotal = 0;
    int bfsSecondFail = 0, bfsSecondTotal = 0;
    for (int t = 0; t < trials; t++) {
        int n = 1 + (int)(rng() % 10);
        Graph g(n);
        int maxEdges = n * n;
        int edgeAttempts = rng() % (maxEdges + 1);
        for (int e = 0; e < edgeAttempts; e++) {
            int u = rng() % n, v = rng() % n;
            if (u != v) g.addEdge(u, v);
        }
        auto correct = canonical(kosaraju(g));

        brokenTotal++;
        if (canonical(kosaraju_brokenBFSFirstPass(g)) != correct) brokenFail++;

        bfsSecondTotal++;
        if (canonical(kosaraju_BFSSecondPass(g)) != correct) bfsSecondFail++;
    }
    cout << "\n[experiment] 第一遍用 BFS (出队顺序当 finish time): "
         << brokenFail << "/" << brokenTotal << " 组随机图出错\n";
    cout << "[experiment] 第二遍用 BFS 队列代替栈:                "
         << bfsSecondFail << "/" << bfsSecondTotal << " 组随机图出错\n";

    return 0;
}

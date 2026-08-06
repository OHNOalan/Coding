#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ---------------------------------------------------------------------------
// 这道题在一个 n x m 的网格上，格子值 > 0 表示「特殊格」（带颜色/权值），值为 0
// 表示空格。要统计满足某种「几何可见性」约束的同值特殊格对数。
//
// 整体分三步：
//   1. precompute()          —— 预处理每个格子的一系列辅助信息（左右/上下最近特殊格、
//                               单调栈邻居、纵向管辖区间等），并建列稀疏表。
//   2. countPairsByEvents()  —— 按行扫描，把「查询/插入」编码成事件，按颜色分桶，
//                               用带惰性清空的二维树状数组做二维支配计数。
//   3. countPairsByColumns()  —— 按列扫描，直接暴力枚举候选点对并做几何校验。
// 最终答案 = 第 2 步计数 + 第 3 步计数。
// ---------------------------------------------------------------------------


// ===========================================================================
// 带惰性清空的二维树状数组。
//
// clear() 通过递增一个「纪元」编号，在 O(1) 内把整棵树逻辑清空：之后的 add/query
// 只会看到本纪元内写入的点。add(row,col) 插入一个点，query(row,col) 统计本纪元内
// 已插入的、满足 row' <= row 且 col' >= col 的点数（二维支配计数）。
// 坐标经过 (row+1, n-col) 变换后落入标准的 [1,n] 前缀维护上。
// ===========================================================================
class Fenwick2D {
public:
    explicit Fenwick2D(int n) :
        n_(n), epoch_(0), tree_(n + 1, vector<int>(n + 1, 0)), stamp_(n + 1, vector<int>(n + 1, 0)) {}

    // 逻辑清空（进入下一个纪元）。
    void clear() { ++epoch_; }

    // 插入一个点。
    void add(int row, int col) {
        int x = row + 1;
        int y = n_ - col;
        for (int i = x; i <= n_; i += i & -i)
            for (int j = y; j <= n_; j += j & -j) {
                if (stamp_[i][j] != epoch_) { // 本纪元第一次触碰 -> 先重置
                    stamp_[i][j] = epoch_;
                    tree_[i][j] = 0;
                }
                ++tree_[i][j];
            }
    }

    // 统计本纪元内 row' <= row 且 col' >= col 的已插入点数。
    int query(int row, int col) const {
        int x = row + 1;
        int y = n_ - col;
        int res = 0;
        for (int i = x; i > 0; i -= i & -i)
            for (int j = y; j > 0; j -= j & -j)
                if (stamp_[i][j] == epoch_) res += tree_[i][j];
        return res;
    }

private:
    int n_;
    int epoch_;
    vector<vector<int>> tree_;
    vector<vector<int>> stamp_; // 每个结点上一次被写入的纪元编号
};


// ===========================================================================
// 列向稀疏表：对每一列，在「行」这一维上维护区间最大值。
//
// 第 0 层就是原始数据（可读写），build() 之后 rangeMax(lo,hi,col) 返回第 col 列上
// 行区间 [lo,hi] 的最大值。数据布局为扁平的 table_[level][row*cols + col]。
// ===========================================================================
class ColumnSparseTable {
public:
    ColumnSparseTable(int rows, int cols) : rows_(rows), cols_(cols) {
        levels_ = 1;
        while ((1 << levels_) <= rows_) ++levels_;
        // 默认值 = cols_（等价于原代码里「不存在右侧特殊格」的哨兵 m）
        table_.assign(levels_, vector<int>(rows_ * cols_, cols_));
        log2_.assign(rows_ + 1, 0);
        for (int i = 2; i <= rows_; i++) log2_[i] = log2_[i / 2] + 1;
    }

    // 第 0 层原始数据的读写访问。
    int &base(int row, int col) { return table_[0][row * cols_ + col]; }
    int base(int row, int col) const { return table_[0][row * cols_ + col]; }

    // 在 base 填好后调用，构建上层。
    void build() {
        for (int k = 1; k < levels_; k++) {
            int half = 1 << (k - 1);
            int len = 1 << k;
            for (int i = 0; i + len <= rows_; i++)
                for (int c = 0; c < cols_; c++)
                    table_[k][i * cols_ + c] = max(table_[k - 1][i * cols_ + c], table_[k - 1][(i + half) * cols_ + c]);
        }
    }

    // 第 col 列上，行区间 [lo, hi] 的最大值（要求 lo <= hi）。
    int rangeMax(int lo, int hi, int col) const {
        int k = log2_[hi - lo + 1];
        return max(table_[k][lo * cols_ + col], table_[k][(hi - (1 << k) + 1) * cols_ + col]);
    }

private:
    int rows_, cols_, levels_;
    vector<vector<int>> table_;
    vector<int> log2_;
};


// ===========================================================================
// 求解器：每个测试用例一个实例，持有该用例的全部状态。
// ===========================================================================
class Solver {
    // 第 3 步枚举点对时用到的候选结点。
    struct Node {
        int id; // row * m + col
        int row, col;
        int leftBound; // = hLeft[id]
        int value; // = grid[id]
    };

    // 二维支配计数用的事件。
    struct Event {
        int seg; // 段编号：段变化时开启新的树状数组纪元
        bool isAdd; // true = 插入, false = 查询
        int x, y; // 树状数组坐标
    };

public:
    Solver(int n, int m, const vector<int> &grid) :
        n_(n), m_(m), grid_(grid), hLeft_(n * m, 0), topBound_(n * m, 0), botBound_(n * m, 0), upNearest_(n * m, -1),
        downNearest_(n * m, -1), prevGE_(n * m, -1), nextGT_(n * m, n), st_(n, m) {}

    ll solve() {
        precompute();
        return countPairsByEvents() + countPairsByColumns();
    }

private:
    // ---- 预处理 --------------------------------------------------------
    void precompute() {
        // 逐行：nearRight（本行中严格右侧最近特殊格的列，无则 m）与 hLeft。
        for (int i = 0; i < n_; i++) {
            int p = m_;
            for (int j = m_ - 1; j >= 0; j--) {
                st_.base(i, j) = p; // nearRight
                if (grid_[i * m_ + j] > 0) p = j;
            }
            int last = -1;
            for (int j = 0; j < m_; j++) {
                int id = i * m_ + j;
                if (grid_[id] > 0) {
                    hLeft_[id] = last + 1; // 上一个同行特殊格列 + 1
                    last = j;
                }
            }
        }

        // 逐列：上下最近特殊格、纵向管辖区间、以及基于 nearRight 的单调栈邻居。
        vector<int> stk(n_);
        for (int j = 0; j < m_; j++) {
            // upNearest：严格上方最近特殊格行，无则 -1
            int last = -1;
            for (int i = 0; i < n_; i++) {
                int id = i * m_ + j;
                upNearest_[id] = last;
                if (grid_[id] > 0) last = i;
            }
            // downNearest：严格下方最近特殊格行，无则 -1
            last = -1;
            for (int i = n_ - 1; i >= 0; i--) {
                int id = i * m_ + j;
                downNearest_[id] = last;
                if (grid_[id] > 0) last = i;
            }

            // 本列特殊格的行列表，用于计算纵向管辖区间 [topBound, botBound]。
            vector<int> rows;
            for (int i = 0; i < n_; i++)
                if (grid_[i * m_ + j] > 0) rows.push_back(i);
            for (int k = 0; k < (int) rows.size(); k++) {
                int id = rows[k] * m_ + j;
                topBound_[id] = k ? rows[k - 1] + 1 : 0;
                botBound_[id] = (k + 1 < (int) rows.size()) ? rows[k + 1] - 1 : n_ - 1;
            }

            // prevGE：上方最近的、nearRight >= 本行 的行（严格小于时出栈）。
            int top = 0;
            for (int i = 0; i < n_; i++) {
                int id = i * m_ + j;
                while (top && st_.base(stk[top - 1], j) < st_.base(i, j)) top--;
                prevGE_[id] = top ? stk[top - 1] : -1;
                stk[top++] = i;
            }
            // nextGT：下方最近的、nearRight > 本行 的行（小于等于时出栈）。
            top = 0;
            for (int i = n_ - 1; i >= 0; i--) {
                int id = i * m_ + j;
                while (top && st_.base(stk[top - 1], j) <= st_.base(i, j)) top--;
                nextGT_[id] = top ? stk[top - 1] : n_;
                stk[top++] = i;
            }
        }

        st_.build();
    }

    // ---- 第 2 步：事件扫描 + 二维树状数组 -------------------------------
    ll countPairsByEvents() {
        ll total = 0;
        Fenwick2D bit(n_);
        vector<vector<Event>> buckets(n_ * m_ + 1); // 按颜色分桶
        vector<int> touched; // 本行被用到的颜色

        for (int z = 0; z < n_; z++) {
            touched.clear();
            generateEvents(z, buckets, touched);

            for (int val: touched) {
                auto &events = buckets[val];
                int curSeg = -1;
                for (const Event &ev: events) {
                    if (ev.seg != curSeg) { // 进入新段 -> 换纪元（逻辑清空）
                        curSeg = ev.seg;
                        bit.clear();
                    }
                    if (ev.isAdd)
                        bit.add(ev.x, ev.y);
                    else
                        total += bit.query(ev.x, ev.y);
                }
                events.clear(); // 归还桶，供下一行复用
            }
        }
        return total;
    }

    // 为第 z 行生成事件（对应原 makeEvent）。
    // 注意 seg 的更新时机：特殊格 now 的「查询」用旧 seg，随后 seg++，其「插入」用新
    // seg，因此同一个特殊格的查询与插入落在不同段（不同纪元）里。
    void generateEvents(int z, vector<vector<Event>> &buckets, vector<int> &touched) {
        auto emit = [&](int val, int seg, bool isAdd, int x, int y) {
            if (buckets[val].empty()) touched.push_back(val);
            buckets[val].push_back(Event{seg, isAdd, x, y});
        };

        int seg = 0;
        for (int c = 0; c < m_; c++) {
            int now = z * m_ + c;

            // 找出「负责」本格的特殊格：本格若特殊则就是自己，否则取上下最近的特殊格。
            int owners[2];
            int k = 0;
            if (grid_[now] > 0) {
                owners[k++] = now;
            } else {
                if (upNearest_[now] != -1) owners[k++] = upNearest_[now] * m_ + c;
                if (downNearest_[now] != -1) owners[k++] = downNearest_[now] * m_ + c;
            }

            // 查询事件（在可能的 seg++ 之前发出）
            for (int i = 0; i < k; i++) {
                int p = owners[i];
                emit(grid_[p], seg, false, topBound_[p], botBound_[p]);
            }

            if (grid_[now] > 0) seg++;

            // 插入事件
            for (int i = 0; i < k; i++) {
                int p = owners[i];
                int U = topBound_[p], D = botBound_[p];
                int pg = prevGE_[now];
                int L = (pg >= U) ? pg + 1 : U;
                int ng = nextGT_[now];
                int R = (ng <= D) ? ng - 1 : D;
                int x = (L == U) ? 0 : L;
                int y = (R == D) ? n_ - 1 : R;
                emit(grid_[p], seg, true, x, y);
            }
        }
    }

    // ---- 第 3 步：逐列枚举候选点对 -------------------------------------
    ll countPairsByColumns() {
        ll count = 0;
        vector<int> lastCol(n_, -1); // lastCol[i] = i 行左侧最近特殊格所在列
        vector<int> good(n_ * n_, -1); // good[i*n+j]：区间无阻挡的最新列
        vector<int> pref(n_ + 1);
        vector<int> rows; // 当前列的特殊格行
        vector<Node> nodes;
        nodes.reserve(2 * n_ + 2);

        for (int c = 0; c < m_; c++) {
            rows.clear();
            for (int i = 0; i < n_; i++)
                if (grid_[i * m_ + c] > 0) rows.push_back(i);
            int sz = rows.size();

            // 枚举相邻两个特殊格之间（含上下边界）的每一段。
            for (int g = 0; g <= sz; g++) {
                int lo = (g == 0) ? 0 : rows[g - 1] + 1;
                int hi = (g == sz) ? n_ - 1 : rows[g] - 1;

                nodes.clear();
                if (g > 0) pushNode(nodes, rows[g - 1], c); // 上侧夹住的特殊格
                if (g < sz) pushNode(nodes, rows[g], c); // 下侧夹住的特殊格
                for (int i = lo; i <= hi; i++) {
                    if (lastCol[i] != -1) pushNode(nodes, i, lastCol[i]); // 左侧最近
                    int nr = st_.base(i, c);
                    if (nr < m_) pushNode(nodes, i, nr); // 右侧最近
                }

                // 暴力枚举同值点对并做几何校验。
                for (int i = 0; i < (int) nodes.size(); i++) {
                    for (int j = 0; j < i; j++) {
                        const Node &x = nodes[i];
                        const Node &y = nodes[j];
                        if (x.value != y.value) continue;

                        int r1 = min(x.row, y.row);
                        int r2 = max(x.row, y.row);
                        int L = max(x.leftBound, y.leftBound);
                        if (good[r1 * n_ + r2] >= L) continue;

                        if (x.col != y.col) {
                            int id1, id2, c1, c2;
                            if (x.col < y.col) {
                                id1 = x.id;
                                id2 = y.id;
                                c1 = x.col;
                                c2 = y.col;
                            } else {
                                id1 = y.id;
                                id2 = x.id;
                                c1 = y.col;
                                c2 = x.col;
                            }
                            int U = max(topBound_[id1], topBound_[id2]);
                            int D = min(botBound_[id1], botBound_[id2]);
                            if (U <= D && st_.rangeMax(U, D, c1) >= c2) continue;
                        }
                        count++;
                    }
                }
            }

            // 更新 good[]：行 i..j 之间（开区间）本列无特殊格时记为 c。
            pref[0] = 0;
            for (int i = 0; i < n_; i++) pref[i + 1] = pref[i] + (grid_[i * m_ + c] > 0 ? 1 : 0);
            for (int i = 0; i < n_; i++)
                for (int j = i; j < n_; j++)
                    if (i == j || pref[j] - pref[i + 1] == 0) good[i * n_ + j] = c;

            // 更新每行「左侧最近特殊格列」。
            for (int r: rows) lastCol[r] = c;
        }
        return count;
    }

    void pushNode(vector<Node> &nodes, int row, int col) {
        int id = row * m_ + col;
        nodes.push_back(Node{id, row, col, hLeft_[id], grid_[id]});
    }

    // ---- 状态 ----------------------------------------------------------
    int n_, m_;
    vector<int> grid_; // grid_[r*m+c]，0 = 空，>0 = 特殊格的值

    vector<int> hLeft_; // 特殊格：同行上一个特殊格列 + 1
    vector<int> topBound_; // 特殊格：纵向管辖区间上界
    vector<int> botBound_; // 特殊格：纵向管辖区间下界
    vector<int> upNearest_; // 严格上方最近特殊格行（-1 无）
    vector<int> downNearest_; // 严格下方最近特殊格行（-1 无）
    vector<int> prevGE_; // nearRight 单调栈：上方最近 >= 者的行（-1 无）
    vector<int> nextGT_; // nearRight 单调栈：下方最近 >  者的行（n 无）

    ColumnSparseTable st_; // 建在 nearRight 上的列稀疏表
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> grid(n * m);
        for (int &x: grid) cin >> x;
        Solver solver(n, m, grid);
        cout << solver.solve() << "\n";
    }
    return 0;
}

#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9;

struct Query {
    int to, id;
};

struct CentroidDecomposition {
    int n;
    vector<int> a;
    vector<vector<int>> g;
    vector<vector<Query>> que;
    vector<long long> ANS;

    // 点分治框架核心数组
    vector<bool> vis;
    vector<int> siz;
    vector<int> nodes;

    // 路径状态维护数组
    vector<int> col; // 节点所在的子树分支颜色
    vector<int> sz; // 节点到重心的距离（节点数）
    vector<int> W; // 从重心出发，不出现重复 bit 的最大距离
    vector<long long> ans; // 严格在当前分支内的合法子段总数
    vector<vector<int>> mn; // mn[v][i]: bit i 在重心到 v 的路径上最早出现的距离

    int E = 0;

    CentroidDecomposition(int n, int q) :
        n(n), a(n + 1), g(n + 1), que(n + 1), ANS(q, -1), vis(n + 1, false), siz(n + 1, 0), col(n + 1, 0), sz(n + 1, 0),
        W(n + 1, 0), ans(n + 1, 0), mn(n + 1, vector<int>(20, INF)) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void add_query(int u, int v, int id) { que[u].push_back({v, id}); }

    void get_centroid(int u, int p, int total, int &centroid) {
        siz[u] = 1;
        int max_sub = 0;
        for (int v: g[u]) {
            if (v != p && !vis[v]) {
                get_centroid(v, u, total, centroid);
                siz[u] += siz[v];
                max_sub = max(max_sub, siz[v]);
            }
        }
        max_sub = max(max_sub, total - siz[u]);
        if (max_sub <= total / 2) centroid = u;
    }

    void get_nodes(int u, int p) {
        nodes.push_back(u);
        for (int v: g[u]) {
            if (v != p && !vis[v]) {
                get_nodes(v, u);
            }
        }
    }

    void dfs_calc(int v, int p, int color, array<int, 20> s, long long an, int depth, int mx) {
        sz[v] = depth;
        col[v] = color;
        W[v] = W[p];
        for (int i = 0; i < 20; ++i) mn[v][i] = mn[p][i];

        for (int i = 0; i < 20; ++i) {
            if ((a[v] >> i) & 1) {
                if (mn[v][i] != INF) W[v] = min(W[v], depth);
                mn[v][i] = min(mn[v][i], depth);
                mx = max(mx, s[i]);
                s[i] = depth;
            }
        }
        an += depth - mx;
        ans[v] = an; // 累加单侧的合法子段数

        for (int u: g[v]) {
            if (u != p && !vis[u]) {
                dfs_calc(u, v, color, s, an, depth + 1, mx);
            }
        }
    }

    // 核心 3：合并跨越重心 C 的两个分支 x 和 y
    long long calc_crossing(int x, int y, int C) {
        if (x == C) {
            int e = min(W[y], sz[y] + 1);
            for (int i = 0; i < 20; ++i)
                if ((a[C] >> i) & 1) e = min(e, mn[y][i]);
            return e;
        } else if (y == C) {
            int e = min(W[x], sz[x] + 1);
            for (int i = 0; i < 20; ++i)
                if ((a[C] >> i) & 1) e = min(e, mn[x][i]);
            return e;
        } else {
            vector<pair<int, int>> upd;
            for (int i = 0; i < 20; ++i) {
                if (mn[x][i] != INF) upd.push_back({mn[x][i], i});
            }
            sort(upd.begin(), upd.end());

            int e = min(W[y], sz[y] + 1);
            for (int i = 0; i < 20; ++i) {
                if ((a[C] >> i) & 1) e = min(e, mn[y][i]);
            }

            long long cross_ans = 0;
            int p = 0;
            bool flag = true;

            // 随着向 x 分支延伸，y 分支受到的 bit 冲突限制越来越严
            for (auto [dist, bit]: upd) {
                if (dist >= W[x]) {
                    cross_ans += 1LL * (W[x] - p) * e;
                    flag = false;
                    break;
                }
                cross_ans += 1LL * (dist - p) * e;
                if ((a[C] >> bit) & 1) {
                    flag = false;
                    break;
                }
                e = min(e, mn[y][bit]);
                p = dist;
            }
            if (flag) {
                cross_ans += 1LL * (min(W[x], sz[x] + 1) - p) * e;
            }
            return cross_ans;
        }
    }

    void solve(int u, int total) {
        int centroid = 0;
        get_centroid(u, 0, total, centroid);
        get_centroid(centroid, 0, total, centroid);
        int C = centroid;

        nodes.clear();
        get_nodes(C, 0);

        vis[C] = true;
        int S = E;

        col[C] = ++E;
        W[C] = INF;
        ans[C] = 0;
        fill(mn[C].begin(), mn[C].end(), INF);
        sz[C] = 0;

        for (int v: g[C]) {
            if (!vis[v]) {
                ++E;
                array<int, 20> s = {0};
                dfs_calc(v, C, E, s, 0, 1, 0);
            }
        }

        for (int x: nodes) {
            for (auto &q: que[x]) {
                int y = q.to, id = q.id;
                if (col[x] > S && col[y] > S && col[x] != col[y]) {
                    ANS[id] = ans[x] + ans[y] + calc_crossing(x, y, C);
                }
            }
        }

        for (int v: g[C]) {
            if (!vis[v]) {
                solve(v, siz[v]);
            }
        }
    }

    void run() {
        solve(1, n);
        for (int i = 0; i < ANS.size(); ++i) {
            cout << ANS[i] << '\n';
        }
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    CentroidDecomposition cd(n, q);
    for (int i = 1; i <= n; ++i) {
        cin >> cd.a[i];
    }
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        cd.add_edge(u, v);
    }
    for (int i = 0; i < q; ++i) {
        int x, y;
        cin >> x >> y;
        cd.add_query(x, y, i);
        cd.add_query(y, x, i); // 无向图查询，双向挂载
    }
    cd.run();
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) solve();
    return 0;
}

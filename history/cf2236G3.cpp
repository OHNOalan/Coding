#include <bits/stdc++.h>

using namespace std;

struct HLD {
    int n, timer;
    vector<vector<int>> g;
    vector<int> siz, dep, par, son, top, dfn, rnk;

    HLD(int n) :
        n(n), timer(0), g(n + 1), siz(n + 1), dep(n + 1), par(n + 1), son(n + 1, 0), top(n + 1), dfn(n + 1),
        rnk(n + 1) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build(int root = 1) {
        auto dfs1 = [&](auto &self, int u, int p, int d) -> void {
            siz[u] = 1;
            par[u] = p;
            dep[u] = d;
            int max_sub = 0;
            for (int v: g[u]) {
                if (v != p) {
                    self(self, v, u, d + 1);
                    siz[u] += siz[v];
                    if (siz[v] > max_sub) {
                        max_sub = siz[v];
                        son[u] = v;
                    }
                }
            }
        };
        dfs1(dfs1, root, 0, 1);

        auto dfs2 = [&](auto self, int u, int t) -> void {
            top[u] = t;
            dfn[u] = ++timer;
            rnk[timer] = u;

            if (!son[u]) return;

            self(self, son[u], t);

            for (int v: g[u]) {
                if (v != par[u] && v != son[u]) {
                    self(self, v, v);
                }
            }
        };
        dfs2(dfs2, root, root);
    }
};

struct SegTree {
    struct Node {
        int cnt = 0;
    };

    int n;
    std::vector<Node> tree;
    const std::vector<int> &a_dfn;

    SegTree(int n, const std::vector<int> &a_dfn) : n(n), tree(4 * n + 1), a_dfn(a_dfn) { build(1, 1, n); }
    void push_up(int p) { tree[p].cnt = tree[p << 1].cnt + tree[p << 1 | 1].cnt; }

    void build(int p, int l, int r) {
        if (l == r) {
            tree[p].cnt = (a_dfn[l] != 0);
            return;
        }
        int mid = l + (r - l) / 2;
        build(p << 1, l, mid);
        build(p << 1 | 1, mid + 1, r);
        push_up(p);
    }

    int query_cnt(int p, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return tree[p].cnt;
        int mid = l + (r - l) / 2;
        return query_cnt(p << 1, l, mid, ql, qr) + query_cnt(p << 1 | 1, mid + 1, r, ql, qr);
    }

    int get_cnt(int ql, int qr) { return query_cnt(1, 1, n, ql, qr); }

    void collect_down(int p, int l, int r, int ql, int qr, std::vector<int> &res, int limit) {
        if (ql > r || qr < l || tree[p].cnt == 0 || (int) res.size() >= limit) return;
        if (l == r) {
            res.push_back(l);
            return;
        }
        int mid = l + (r - l) / 2;
        collect_down(p << 1, l, mid, ql, qr, res, limit);
        collect_down(p << 1 | 1, mid + 1, r, ql, qr, res, limit);
    }
    void get_non_zeros(int ql, int qr, std::vector<int> &res, int limit = INT_MAX) {
        if (ql > qr) return;
        collect_down(1, 1, n, ql, qr, res, limit);
    }
};

const int LG = 21;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    HLD hld(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        hld.add_edge(u, v);
    }
    hld.build();

    vector<int> a_dfn(n + 1);
    for (int i = 1; i <= n; ++i) {
        a_dfn[hld.dfn[i]] = a[i];
    }
    SegTree seg(n, a_dfn);

    vector<int> bad(n + 1, 0);
    vector<int> last_seen(20, 0);
    vector<int> dfs_path(n + 1, 0);

    [&](this auto self, int u, int p, int d, int max_conf) -> void {
        dfs_path[d] = u;
        int cur_conf = max_conf;
        vector<pair<int, int>> rollbacks;
        for (int b = 0; b < 20; b++) {
            if ((a[u] >> b) & 1) {
                if (last_seen[b] > 0) cur_conf = max(cur_conf, last_seen[b]);
                rollbacks.push_back({b, last_seen[b]});
                last_seen[b] = d;
            }
        }
        bad[u] = (cur_conf == 0) ? 0 : dfs_path[cur_conf];
        for (int v: hld.g[u])
            if (v != p) self(v, u, d + 1, cur_conf);
        for (auto [b, old_val]: rollbacks) last_seen[b] = old_val; // 回溯
    }(1, 0, 1, 0);

    vector<long long> sum_vertical(n + 1, 0);
    [&](this auto self, int u, int p) -> void {
        sum_vertical[u] = sum_vertical[p] + (hld.dep[u] - hld.dep[bad[u]]);
        for (int v: hld.g[u])
            if (v != p) self(v, u);
    }(1, 0);

    auto get_vertical_hospitable = [&](int bottom, int top) -> long long {
        if (hld.dep[bottom] <= hld.dep[top]) return 0;
        return sum_vertical[bottom] - sum_vertical[top];
    };

    while (q--) {
        int x, y;
        cin >> x >> y;
        if (hld.dep[x] < hld.dep[y]) swap(x, y);

        int u = x, v = y;
        vector<pair<int, int>> segsX, segsY;

        while (hld.top[u] != hld.top[v]) {
            if (hld.dep[hld.top[u]] > hld.dep[hld.top[v]]) {
                segsX.push_back({hld.dfn[hld.top[u]], hld.dfn[u]});
                u = hld.par[hld.top[u]];
            } else {
                segsY.push_back({hld.dfn[hld.top[v]], hld.dfn[v]});
                v = hld.par[hld.top[v]];
            }
        }
        int z;
        if (hld.dep[u] > hld.dep[v]) {
            segsX.push_back({hld.dfn[v] + 1, hld.dfn[u]});
            z = v;
        } else {
            if (v != u) segsY.push_back({hld.dfn[u] + 1, hld.dfn[v]});
            z = u;
        }

        int totX = 0, totY = 0;
        for (auto [ql, qr]: segsX) totX += seg.get_cnt(ql, qr);
        for (auto [ql, qr]: segsY) totY += seg.get_cnt(ql, qr);

        reverse(segsX.begin(), segsX.end());
        reverse(segsY.begin(), segsY.end());

        vector<int> resX_dfn, resY_dfn;
        for (auto [ql, qr]: segsX) seg.get_non_zeros(ql, qr, resX_dfn, LG);
        for (auto [ql, qr]: segsY) seg.get_non_zeros(ql, qr, resY_dfn, LG);

        int xx = (totX <= LG) ? x : hld.rnk[resX_dfn.back()];
        int yy = (totY <= LG) ? y : hld.rnk[resY_dfn.back()];

        reverse(resX_dfn.begin(), resX_dfn.end());

        vector<pair<int, int>> path;
        int current_pos = hld.dep[z] - hld.dep[xx];

        auto add_node = [&](int node, int p) {
            int zeros = p - current_pos;
            if (zeros > 0) path.push_back({0, zeros});
            path.push_back({a[node], 1});
            current_pos = p + 1;
        };

        for (int dfn: resX_dfn) {
            int node = hld.rnk[dfn];
            add_node(node, hld.dep[z] - hld.dep[node]);
        }
        if (a[z] != 0) {
            add_node(z, 0);
        }
        for (int dfn: resY_dfn) {
            int node = hld.rnk[dfn];
            add_node(node, hld.dep[node] - hld.dep[z]);
        }

        int end_pos = hld.dep[yy] - hld.dep[z];
        int zeros_after = end_pos - current_pos + 1;
        if (zeros_after > 0) path.push_back({0, zeros_after});

        long long ans = get_vertical_hospitable(x, xx) + get_vertical_hospitable(y, yy);
        int m = path.size();
        for (int i = 0, j = 0, Xor = 0, Sum = 0, tc = 0; j < m; j++) {
            auto [val, c] = path[j];
            Xor ^= val;
            Sum += val;
            while (Xor != Sum) {
                Xor ^= path[i].first;
                Sum -= path[i].first;
                tc -= path[i].second;
                i++;
            }
            ans += 1LL * c * (2LL * tc + 1 + c) / 2;
            tc += c;
        }
        cout << ans << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

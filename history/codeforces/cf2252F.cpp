#include <bits/stdc++.h>

using namespace std;

constexpr int LOG = 19;

void solve() {
    int n;
    cin >> n;
    vector<int> C(n);
    vector<vector<int>> colors(n);
    for (int i = 0; i < n; ++i) {
        cin >> C[i], --C[i];
        colors[C[i]].push_back(i);
    }
    vector<int> K(n);
    for (int i = 0; i < n; ++i) cin >> K[i];
    vector<vector<int>> g(n);
    for (int i = 0, u, v; i < n - 1; ++i) {
        cin >> u >> v, --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int timer = 0;
    vector<int> tin(n), tout(n), depth(n);
    vector<vector<int>> up(n, vector<int>(LOG));
    [&](this auto &&self, int u, int p, int d) -> void {
        tin[u] = ++timer;
        depth[u] = d;
        up[u][0] = p;
        for (int i = 1; i < LOG; ++i) up[u][i] = up[up[u][i - 1]][i - 1];
        for (int v: g[u]) {
            if (v != p) self(v, u, d + 1);
        }
        tout[u] = ++timer;
    }(0, 0, 0);

    auto is_ancestor = [&](int u, int v) -> bool { return tin[u] <= tin[v] && tout[u] >= tout[v]; };

    auto lca = [&](int u, int v) -> int {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int i = 0; i < LOG; ++i)
            if (diff >> i & 1) u = up[u][i];
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; --i) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    };

    vector<long long> ans(n, -1);
    vector<int> cnt(n, 0);
    for (int c = 0; c < n; c++) {
        if (colors[c].empty()) continue;
        int m = colors[c].size();
        int k = K[c];

        vector<int> nodes = colors[c];
        ranges::sort(nodes, [&](int a, int b) -> bool { return tin[a] < tin[b]; });

        for (int i = 0; i < m - 1; i++) nodes.push_back(lca(nodes[i], nodes[i + 1]));

        {
            ranges::sort(nodes, [&](int a, int b) -> bool { return tin[a] < tin[b]; });
            auto [st, ed] = ranges::unique(nodes);
            nodes.erase(st, ed);
        }

        for (int u: colors[c]) cnt[u] = 1;

        vector<int> st;
        vector<pair<int, int>> vt_edges;
        for (int u: nodes) {
            while (!st.empty() && !is_ancestor(st.back(), u)) st.pop_back();
            if (!st.empty()) vt_edges.push_back({st.back(), u});
            st.push_back(u);
        }

        long long total_dist = 0;
        vector<pair<long long, long long>> edge_weights;

        for (int i = (int) vt_edges.size() - 1; i >= 0; --i) {
            auto [p, u] = vt_edges[i];
            cnt[p] += cnt[u];
            long long w = min((long long) cnt[u], (long long) m - cnt[u]);
            long long L = depth[u] - depth[p];
            total_dist += w * L;
            edge_weights.push_back({w, L});
        }

        ranges::sort(edge_weights, greater<>{});
        long long saved = 0;
        long long needed = k - 1;
        for (auto [w, L]: edge_weights) {
            long long take = min(needed, L);
            saved += take * w;
            needed -= take;
            if (needed == 0) break;
        }
        ans[c] = total_dist - saved;
        for (int u: nodes) cnt[u] = 0;
    }
    for (int i = 0; i < n; ++i) cout << ans[i] << " \n"[i == n - 1];
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
    return 0;
}

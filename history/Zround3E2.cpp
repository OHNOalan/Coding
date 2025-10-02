#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<vector<int>> G(n);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        G[u].push_back(v); G[v].push_back(u);
    }

    vector<int> dep(n), sz(n);
    vector<ll> sq(n);
    vector<vector<int>> fa(20, vector<int>(n));
    auto dfs = [&](auto &&self, int u) -> void {
        ll sq1 = 0, sq2 = 0;
        sz[u] = 1;
        for (auto v: G[u]) if (v != fa[0][u]) {
            dep[v] = dep[u] + 1;
            fa[0][v] = u;
            self(self, v);
            sz[u] += sz[v];
            sq2 += 1LL * sz[v] * sz[v];
        }
        sq2 += 1LL * (n - sz[u]) * (n - sz[u]);
        sq1 = 1LL * (n - 1) * (n - 1);
        sq[u] = (sq1 - sq2) / 2 + n - 1;
    };
    dfs(dfs, 0);
    for (int j = 1; j < 20; ++j) for (int u = 0; u < n; ++u) fa[j][u] = fa[j-1][fa[j-1][u]];
    auto ka = [&](int u, int k) -> int {
        int v = u, j = 0;
        while (k) {
            if (k & 1) v = fa[j][v];
            ++j;
            k >>= 1;
        }
        return v;
    };
    auto lca = [&](int u, int v) -> int {
        // if (dep[v] < dep[u]) swap(u, v);
        v = ka(v, dep[v] - dep[u]);
        if (u == v) return u;
        for (int j = 19; j >= 0; --j) {
            if (fa[j][u] != fa[j][v]) {
                u = fa[j][u];
                v = fa[j][v];
            }
        }
        return fa[0][u];
    };

    while (q--) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        if (u == v) {
            cout << sq[u];
        } else {
            if (dep[v] < dep[u]) swap(u, v);
            auto p = lca(u, v);
            int left = sz[v], right = 0;
            if (p == u) {
                int nxt = ka(v, dep[v] - dep[u] - 1);
                right = n - sz[nxt];
            } else right = sz[u];
            cout << 1LL * right * left;
        }
        if (q) cout << ' ';
    }
    cout << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

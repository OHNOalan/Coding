#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<vector<int>> g(n + 1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<i64> f(n + 1), sz(n + 1);
    auto dfs = [&](auto &&dfs, int x, int fa) -> int {
        sz[x] = 1;
        for (int y: g[x])
            if (y != fa) {
                sz[x] += dfs(dfs, y, x);
            }
        if (sz[x] >= k)
            f[x] += n - sz[x];
        return sz[x];
    };
    auto dfs2 = [&](auto &&dfs2, int x, int fa) -> void {
        for (int y: g[x])
            if (y != fa) {
                if (n - sz[y] >= k)
                    f[x] += sz[y];
                dfs2(dfs2, y, x);
            }
    };
    dfs(dfs, 1, 0);
    dfs2(dfs2, 1, 0);

    i64 ans = accumulate(f.begin(), f.end(), 0ll) + n;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
constexpr int mod = 1000000007;

vector<int> decomp(int n, int k) {
    vector<vector<int>> G(n);
    vector<int> ans(n);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    if (k < 0) return ans;
    vector<int> vis(n), sz(n);
    int mn = INT_MAX, c = 0;
    vector<vector<int>> cnt(n);
    vector<int> tot; tot.reserve(n);
    auto find = [&](this auto &&self, int u, int fa, int t) -> void {
        sz[u] = 1;
        int mx = 0;
        for (auto v: G[u]) {
            if (v == fa || vis[v]) continue;
            self(v, u, t);
            mx = max(mx, sz[v]);
            sz[u] += sz[v];
        }
        mx = max(mx, t - sz[u]);
        if (mx < mn) {
            mn = mx;
            c = u;
        }
    };
    auto dfs = [&](this auto &&self, int a, int u, int fa, int d) -> void {
        if (d == cnt[a].size()) cnt[a].push_back(1);
        else cnt[a][d]++;
        if (d == tot.size() - 1) tot.push_back(1);
        else tot[d+1]++;
        for (auto v: G[u]) {
            if (vis[v] || v == fa) continue;
            self(a, v, u, d + 1);
        }
    };
    auto dfs2 = [&](this auto &&self, int a, int u, int fa, int d) -> void {
        int o = k - d - 1;
        if (o < 0) return;
        if (!o) {
            ++ans[u];
            return;
        }
        int x = o >= tot.size() ? tot.back() : tot[o];
        int y = o >= cnt[a].size() ? cnt[a].back() : cnt[a][o-1];
        ans[u] += x - y;
        for (auto v: G[u]) {
            if (vis[v] || v == fa) continue;
            self(a, v, u, d + 1);
        }
    };
    auto work = [&](this auto &&self, int u, int t) -> void {
        mn = INT_MAX;
        find(u, u, t);
        u = c;
        find(u, u, t);
        tot.clear();
        tot.push_back(1);
        for (auto v: G[u]) {
            if (!vis[v]) {
                cnt[v].clear();
                dfs(v, v, u, 0);
            }
        }
        for (int i = 1; i < tot.size(); ++i) tot[i] += tot[i-1];
        ans[u] += k >= tot.size() ? tot.back() : tot[k];
        for (auto v: G[u]) {
            if (!vis[v]) {
                for (int i = 1; i < cnt[v].size(); ++i) cnt[v][i] += cnt[v][i-1];
                dfs2(v, v, u, 0);
            }
        }
        vis[u] = true;
        for (auto v: G[u]) {
            if (!vis[v]) self(v, sz[v]);
        }
    };
    work(0, n);
    return ans;
}

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    auto f1 = decomp(n, k), f2 = decomp(m, k - 1);
    int mx = ranges::max(f2);
    for (auto &v: f1) cout << v + mx << ' ';
    cout << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int T = 1;
    cin >> T;
    while (T--) solve();
    return 0;
}

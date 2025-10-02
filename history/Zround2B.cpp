#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
// static constexpr int MOD = 1e9 + 7;
static constexpr int MOD = 998244353;
static constexpr ll INF = 1e18 + 1;

void solve() {
    ll n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g[v].push_back(u);
    }
    vector<ll> f(n, 1);
    for (int i = 1; i < n; i++) {
        for(int j : g[i]) {
            f[i] += f[j];
        }
        f[i] %= MOD;
    }

    for(int x : f)
        cout << x << " ";
    cout << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
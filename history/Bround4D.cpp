#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5 + 10;
using ll = long long;
constexpr int mod = 998244353;

ll fac[MAXN];

void init() {
    fac[0] = 1;
    for(int i=1; i<MAXN; i++)
        (fac[i] = fac[i-1] * i) %= mod;
}

void solve() {
    int n; cin >> n;
    vector<vector<int>> G(n+1);
    for(int i=0; i<n-1; i++) {
        int u,v; cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    auto dfs = [&](auto&& dfs, int x, int fa) -> ll {
        ll cnt = 0, prod = 1;
        for(int y : G[x]) {
            if(y != fa) {
                (prod *= dfs(dfs, y, x)) %= mod;
                cnt++;
            }
        }
        return prod * fac[cnt] % mod;
    };
    cout << dfs(dfs, 1, 0) << '\n';
}

#define MULTICASE true
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2010;
using ll = long long;

void solve() {
    int n; cin >> n;
    int q; cin >> q;
    vector<vector<int>> g(n+1);
    vector<ll> ans(n+1);
    for(int i=1; i<n; i++) {
        int x,y; cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }

    auto dfs = [&](auto&& dfs, int x, int fa) -> int {
        int tot = 0;
        ll sum = 0;
        for(int y: g[x]) {
            if(y != fa) {
                int cnt = dfs(dfs, y, x);
                tot += cnt;
                sum += (ll) cnt * (n - cnt - 1);
            }
        }
        if(fa != 0) {
            sum += (ll) tot * (n - tot - 1);
        }
        sum /= 2;
        sum += n-1;
        ans[x] = sum;
        return tot + 1;
    };
    dfs(dfs, 1, 0);

    while(q--) {
        int u,v; cin >> u >> v;
        cout << ans[u] << ' ';
    }
    cout << endl;
}

#define MULTICASE true
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
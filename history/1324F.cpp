#include <bits/stdc++.h>
using namespace std;


using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    vector<vector<int> > G(n + 1);
    for (int i = 1; i <= n - 1; i++) {
        int u, v;
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    vector<int> ans(n + 1);
    auto dfs = [&](auto &&dfs, int x, int fa) -> int {
        int res = 2 * a[x] - 1;
        for (int y: G[x])
            if (y != fa) {
                res += dfs(dfs, y, x);
            }
        ans[x] = res;
        return max(res, 0);
    };
    dfs(dfs, 1, 0);
    auto reroot = [&](auto &&reroot, int x, int fa) -> void {
        for (int y: G[x]) if (y != fa) {
            if(ans[y] > 0) {
                ans[y] = max(0, ans[x] - ans[y]) + ans[y];
            } else {
                ans[y] = max(0, ans[x]) + ans[y];
            }
            reroot(reroot, y, x);
        }
    };
    reroot(reroot, 1, 0);
    for(int i = 1; i <= n; i++) {
        cout << ans[i] << " ";
    }
    cout << '\n';
}

#define MULTICASE false

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

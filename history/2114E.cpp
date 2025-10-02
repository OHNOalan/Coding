#include <bits/stdc++.h>
using namespace std;
int init = []() -> int { return 0; }();

void solve() {
    int n;
    cin >> n;
    vector a(n, array<long long, 2>{});
    for (int i = 0; i < n; i++) {
        cin >> a[i][0];
        a[i][1] = a[i][0];
    }
    vector G(n, vector<int>());
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    auto dfs = [&](auto &&dfs, int x, int fa) -> void {
        for (int y: G[x])
            if (y != fa) {
                a[y][0] = max(a[y][0], a[y][0] - a[x][1]);
                a[y][1] = min(a[y][1], a[y][1] - a[x][0]);
                dfs(dfs, y, x);
            }
    };
    dfs(dfs, 0, -1);
    for (auto &[mx, mn]: a)
        cout << mx << " ";
    cout << '\n';
}


#define MULTICASE true

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

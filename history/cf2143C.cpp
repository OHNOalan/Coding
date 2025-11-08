#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> deg(n);
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++) {
        int u, v, x, y;
        cin >> u >> v >> x >> y;
        u--, v--;
        if (x > y) {
            swap(u, v);
            swap(x, y);
        }
        g[u].push_back(v);
        deg[v]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (deg[i] == 0) {
            q.push(i);
        }
    }

    int cur = 1;
    vector<int> ans(n, -1);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ans[u] = cur++;
        for (int v: g[u]) {
            if (--deg[v] == 0) {
                q.push(v);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        assert(ans[i] != -1);
        cout << ans[i] << " ";
    }
    cout << '\n';
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

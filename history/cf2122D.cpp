#include <bits/stdc++.h>

using ll = long long;

using namespace std;

const int N = int(2e5) + 99;

int t, n, m;
ll x, y;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> t;
    while (t--) {
        cin >> n >> m;
        vector<vector<int>> g(n + 1);
        for (int i = 0; i < m; i++) {
            cin >> x >> y;
            g[x].push_back(y);
            g[y].push_back(x);
        }
        vector<int> f(n + 1, INT_MAX);
        f[1] = 0;
        for (int t = 0;; t++) {
            vector<int> nf(n + 1, INT_MAX);
            for (int i = 1; i <= n; i++) {
                if (f[i] == INT_MAX)
                    continue;
                int j = g[i][t % g[i].size()];
                nf[i] = min(nf[i], f[i] + 1);
                nf[j] = min(nf[j], f[i]);
            }
            swap(f, nf);
            if (f[n] != INT_MAX) {
                cout << t + 1 << " " << f[n] << '\n';
                break;
            }
        }
    }
    return 0;
}

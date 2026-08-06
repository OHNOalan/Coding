#include <bits/stdc++.h>

using namespace std;

constexpr int mod = 998'244'353;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> g(n);
    for (int i = 0, u, v; i < n - 1; i++) {
        cin >> u >> v, --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int start = n - 1;
    while (g[start].size() != 1) start--;

    vector<int> mx(n, -1);
    [&](this auto &&self, int u, int p) -> int {
        int m1 = -1, m2 = -1;
        for (int v: g[u]) {
            if (v == p) continue;
            int cur = self(v, u);
            if (cur > m1) {
                m2 = m1;
                m1 = cur;
            } else if (cur > m2) {
                m2 = cur;
            }
        }
        mx[u] = (u == n - 1) ? m2 : m1;
        return max(m1, u);
    }(n - 1, -1);

    vector<long long> dp(n), ev(n);
    dp[n - 1] = 1;

    long long sum = 0;
    for (int i = n - 2; i >= start; i--) {
        int y = i + 1;
        if (mx[y] < y) {
            sum = (sum + dp[y] + mod) % mod;
            if (mx[y] >= 0) {
                ev[mx[y]] = (ev[mx[y]] + dp[y]) % mod;
            }
        }
        sum = (sum - ev[i] + mod) % mod;
        dp[i] = sum;
    }
    cout << dp[start] << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

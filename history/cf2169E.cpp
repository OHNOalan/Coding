#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<i64> x(n), y(n), c(n);
    for (auto &i: x) cin >> i;
    for (auto &i: y) cin >> i;
    for (auto &i: c) cin >> i;

    vector<i64> dp(16, LLONG_MIN / 2);
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        auto ndp = dp;
        for (int s = 0; s < 16; s++) {
            long long cost = 0;
            if (s & 1) cost += x[i];
            if (s & 2) cost -= x[i];
            if (s & 4) cost += y[i];
            if (s & 8) cost -= y[i];
            cost *= 2;
            cost -= c[i];
            ndp[s] = max(ndp[s], cost);
            for (int t = 15 - s; t; t = (t - 1) & (15 - s)) {
                ndp[t | s] = max(ndp[t | s], dp[t] + cost);
            }
        }
        dp = std::move(ndp);
    }

    cout << accumulate(c.begin(), c.end(), 0ll) + dp[15] << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), c(n);
    for (int &x: a) cin >> x;
    for (int &x: c) cin >> x;
    vector<i64> dp(n);
    for (int i = 0; i < n; i++) {
        dp[i] = c[i];
        for (int j = 0; j < i; j++)
            if (a[i] >= a[j]) {
                dp[i] = max(dp[i], dp[j] + c[i]);
            }
    }

    i64 ans = accumulate(c.begin(), c.end(), 0ll) - *max_element(dp.begin(), dp.end());
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

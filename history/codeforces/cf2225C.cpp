#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    string s, t;
    cin >> s >> t;

    vector<int> dp(n + 1);
    dp[1] = s[0] != t[0];
    for (int i = 2; i <= n; i++) {
        dp[i] = min(dp[i - 1] + (s[i - 1] != t[i - 1]), dp[i - 2] + (s[i - 1] != s[i - 2]) + (t[i - 1] != t[i - 2]));
    }
    cout << dp[n] << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

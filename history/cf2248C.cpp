#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(2 * n);
    for (int i = 0; i < 2 * n; i++) cin >> a[i], --a[i];
    vector<long long> dp(2 * n + 1), pre(2 * n + 1), lst(n, -1);
    for (int i = 1; i <= 2 * n; i++) {
        int x = a[i - 1];
        if (lst[x] == -1) {
            dp[i] = 1 + pre[i - 1];
        } else {
            dp[i] = max(1 + pre[i - 1], 1ll * (i - lst[x] + 1) * (i - lst[x] + 1) + pre[lst[x] - 1]);
        }
        pre[i] = max(pre[i - 1], dp[i]);
        lst[x] = i;
    }
    cout << pre[2 * n] << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

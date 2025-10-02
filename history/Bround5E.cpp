#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n, m; cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    vector<vector<int>> dp(n+1, vector<int>(m+1));
    dp[0][0] = 1;
    for(int i=0; i<n; i++) {
        int val = INT_MAX;
        for(int j=i; j>=0; j--) {
            val &= a[j];
            for(int k=m; k>=val; --k) dp[i+1][k] |= dp[j][k - val];
        }
    }
    int ans = 0;
    for(int k=0; k<=m; k++) if(dp[n][k] == 1) ans++;
    cout << ans << '\n';
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

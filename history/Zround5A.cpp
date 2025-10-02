#include <bits/stdc++.h>
using namespace std;

using ll = long long;
constexpr int mod = 998244353;
constexpr int maxn = 1000;

int dp[maxn + 1];

void init() {
    dp[0] = 0;
    for(int i=1, sum = 0; i<=maxn; i++) {
        dp[i] = sum + i;
        sum += dp[i];
    }
}

void solve() {
    int n, k; cin >> n >> k;
    int ans = 0;
    for(int i=0; k; i++, k>>=1) {
        if(k & 1) ans += dp[i];
    }
    cout << ans << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

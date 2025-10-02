#include <bits/stdc++.h>
#define MULTICASE true
using namespace std;

using ll = long long;
static constexpr ll MOD = 998244353;

int inv[1024];

void init() {
    inv[1] = 1;
    for(int i=2; i<1024; ++i)
        inv[i] = (MOD - MOD / i) * inv[MOD % i] % MOD;
}

void solve() {
    int m; cin >> m;

    vector<int> a(m);
    for(int i = 0; i < m; i++)
        cin >> a[i];

    vector<ll> dp(m);
    dp[m-1] = 1;
    for(int i=0; i<m-1; i++) {
        int p = 0;
        for(int j=0; j<m-1; j++) {
            if(a[j] > a[j+1]) {
                swap(a[j], a[j+1]);
                p = j+1;
            }
        }
        if(p == 0) break;
        dp[m-1] = dp[m-1] * inv[m] % MOD;
        for(int j=m-2; j>=p; j--) {
            dp[j] = (dp[j+1] + dp[j] * inv[j+1] % MOD) % MOD;
        }
    }
    ll ans = accumulate(dp.begin(), dp.end(), 0LL);
    cout << ans % MOD << '\n';
}

int main() {
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

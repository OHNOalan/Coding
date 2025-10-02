#include <bits/stdc++.h>
using namespace std;

using ll = long long;
constexpr int mod = 998244353;
constexpr int maxn = 64;

ll dp[maxn + 1];

void init() {
    dp[0] = 0;
    for(ll i=1, sum = 0; i<=maxn; i++) {
        dp[i] = sum + i;
        sum += dp[i];
    }
}

void solve() {
    ll n, k; cin >> n >> k;
    ll left = 0;
    for(int i=0; i<n; i++) {
        int x; cin >> x;
        left += 1LL << x;
    }
    ll ans = 0;
    ll right = left + k;
    for(int i=0; right; i++, right>>=1) {
        if(right & 1) ans += dp[i];
    }
    for(int i=0; left; i++, left>>=1) {
        if(left & 1) ans -= dp[i];
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

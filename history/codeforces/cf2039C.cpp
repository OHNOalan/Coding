#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    ll x, m; cin >> x >> m;
    ll k = 0;
    while(x >= (1LL << k)) k++;
    ll u = (1LL << k) - 1;
    ll ans = 0;
    for(ll y = 1; y <= min(u, m); y++) {
        ll z = x ^ y;
        if(z % x == 0 || z % y == 0) ans++;
    }
    if(m <= u) {
        cout << ans << '\n';
        return;
    }
    ll p = m;
    m >>= k;
    m <<= k;
    ll l = ((1LL << k) - 1) / x + 1;
    ll r = (m - 1) / x + 1;
    ans += max(0LL, r - l);
    for(ll mul = r; mul < r + 2; mul++) {
        ll t = (x * mul) ^ x;
        if(t <= p) {
            ans++;
        }
    }
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

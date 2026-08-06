#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    ll x, m; cin >> x >> m;
    ll k = 0;
    while(x >= (1LL << k)) {
        k++;
    }
    m = min(m, (1LL<<k) - 1);
    int ans = 0;
    for(ll y = 1LL; y <= m; y++) {
        ll z = x ^ y;
        if(z && (x % z == 0 || y % z == 0)) ans++;
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

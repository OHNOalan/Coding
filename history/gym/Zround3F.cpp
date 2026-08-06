#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    ll n, x, y;
    cin >> n >> x >> y;
    vector<ll> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    if (!x) {
        cout << a[y-1] << '\n';
        return;
    }
    ll ans = 0;
    for (int i = 0; i < y; ++i) {
        if ((x - 1 & x + y - i - 2) == x - 1) ans ^= a[i];
    }
    cout << ans << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

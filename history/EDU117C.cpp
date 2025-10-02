#include <bits/stdc++.h>
using namespace std;

using ll = long long;
void solve() {
    ll k, x;
    cin >> k >> x;
    if(x > k * (k + 1) / 2) {
        x -= k * (k + 1) / 2;
        int l = 0, r = k-1;
        while(l + 1 < r) {
            ll m = (l + r) >> 1;
            if(x <= (2 * k - m - 1) * m / 2) {
                r = m;
            } else {
                l = m;
            }
        }
        cout << k + r << '\n';
    } else {
        ll l = 0, r = k;
        while(l + 1 < r) {
            ll m = (l + r) >> 1;
            if(x <= m * (m + 1) / 2) {
                r = m;
            } else {
                l = m;
            }
        }
        cout << r << '\n';
    }
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    cin >> t;
    while (t--) solve();
}

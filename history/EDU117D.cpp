#include <bits/stdc++.h>
using namespace std;

using ll = long long;
void solve() {
    ll a, b, x;
    cin >> a >> b >> x;
    if(a < b) swap(a, b);
    while (b) {
        if(x > a) {
            cout << "NO\n";
            return;
        }
        if((a - x) % b == 0) {
            cout << "YES\n";
            return;
        }
        a %= b;
        swap(a, b);
    }
    cout << "NO\n";
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) solve();
}

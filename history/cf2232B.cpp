#include <bits/stdc++.h>

using namespace std;
using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<ll> a(n);
    for (auto &x: a) cin >> x;
    ll ans = 1e9, sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
        ans = min(ans, sum / (i + 1));
        cout << ans << " ";
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

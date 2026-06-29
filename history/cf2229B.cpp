#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<long long> a(n), b(n);
    for (auto &x: a) cin >> x;
    for (auto &x: b) cin >> x;
    long long ans = 0, mx = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > b[i]) swap(a[i], b[i]);
        ans += b[i];
        mx = max(mx, a[i]);
    }
    cout << ans + mx << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

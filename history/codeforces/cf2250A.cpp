#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    if (n % 2) return void(cout << "NO\n");
    int mx = INT_MAX, mn = INT_MIN;
    for (int i = 0; i < n; i += 2) {
        if (a[i] <= a[i + 1]) return void(cout << "NO\n");
        mn = max(mn, a[i + 1]);
        mx = min(mx, a[i]);
    }
    cout << (mn + 1 < mx ? "YES\n" : "NO\n");
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

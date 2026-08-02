#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < m; i++) cin >> b[i];
    if (n < 2 * m) return void(cout << "NO\n");
    ranges::sort(a);
    ranges::sort(b);
    for (int i = 0, j = n - m; i < m; i++, j++) {
        if (a[i] > b[i] || a[j] < b[i]) return void(cout << "NO\n");
    }
    cout << "YES\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

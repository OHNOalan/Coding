#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];
    ranges::sort(a);
    ranges::sort(b);
    if (a == b) return void(cout << "YES\n");
    int x = 0;
    for (int i = 0; i < n; ++i) x ^= a[i] ^ b[i];
    for (int i = 0; i < n; ++i) {
        if (a[i] != x) continue;
        for (int j = 0; j < n; j++) {
            if (j != i) a[j] ^= x;
        }
        ranges::sort(a);
        return void(cout << (a == b ? "YES\n" : "NO\n"));
    }
    cout << "NO\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

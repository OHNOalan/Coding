#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;

    i64 ans = 0;
    int mx = max(a[0], a[1]);
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            ans += max(0, a[i] - mx + 1);
        }
        mx = max(mx, a[i]);
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

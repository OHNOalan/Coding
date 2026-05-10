#include <bits/stdc++.h>

using namespace std;

void solve() {
    int x, y;
    cin >> x >> y;
    int s = x - y;
    int ans = 0;
    for (int i = 1; i <= abs(s); i++) {
        if (s % i == 0) {
            ans++;
        }
    }
    if (s == 0) ans += 1;
    cout << ans << '\n';
    for (int i = 0; i < x; i++) cout << 1 << " ";
    for (int i = 0; i < y; i++) cout << -1 << " ";
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

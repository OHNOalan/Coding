#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int r0, x, d, n;
    cin >> r0 >> x >> d >> n;
    string s;
    cin >> s;
    int i = 0, ans = 0;
    for (; i < n && r0 >= x; i++) {
        if (s[i] == '1') {
            ans++;
            r0 = max(0, r0 - d);
        }
    }
    ans += n - i;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    int ans;
    if (n == 1) {
        ans = 1;
    } else if (n == 2) {
        ans = 9;
    } else {
        ans = max((n * n) * 4 - n - 4, (n * n) * 5 - 5 - 5 * n);
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

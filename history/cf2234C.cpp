#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> h(n);
    for (auto &x: h) cin >> x;
    vector lim(2, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        ranges::fill(lim[0], 0);
        ranges::fill(lim[1], 0);
        for (int j = (i + 1) % n; j != i; ++j, j = j % n) {
            int lst = (j - 1 + n) % n;
            lim[0][j] = max(lim[0][lst], h[lst]);
        }
        for (int j = (i - 1 + n) % n; j != i; --j, j = (j + n) % n) {
            int lst = (j + 1) % n;
            lim[1][j] = max(lim[1][lst], h[j]);
        }
        long long ans = 0;
        for (int j = 0; j < n; ++j) ans += min(lim[0][j], lim[1][j]);
        cout << ans << ' ';
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

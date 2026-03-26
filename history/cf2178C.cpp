#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

// | - - -
// | (+-) | -

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    vector<i64> pre(n + 1);
    for (int i = 0; i < n; i++) {
        pre[i + 1] = pre[i] + abs(a[i]);
    }
    vector<i64> suf(n + 1);
    for (int i = n - 1; i >= 0; i--) {
        suf[i] = suf[i + 1] - a[i];
    }
    i64 ans = suf[1];
    for (int i = 1; i < n; i++) {
        ans = max(ans, a[0] + (pre[i] - pre[1]) + suf[i + 1]);
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

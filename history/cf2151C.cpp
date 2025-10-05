#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(2 * n);
    for (int &x: a) cin >> x;

    vector<i64> pre(2 * n);
    for (int i = 0; i < n; i++) {
        pre[i] = a[i];
        if (i)
            pre[i] += pre[i - 1];
    }

    vector<i64> suf(2 * n);
    for (int i = 2 * n - 1; i >= n; i--) {
        suf[i] = a[i];
        if (i != 2 * n - 1)
            suf[i] += suf[i + 1];
    }

    vector<i64> pre2(2 * n + 1);
    for (int i = 0; i < n * 2; i++) {
        pre2[i + 1] = a[i];
        if (i > 1)
            pre2[i + 1] += pre2[i - 1];
    }

    // ak+1 - ak + ak+3 - ak+2 ... a(2n-k-1) - a(2n-k-2)
    for (int k = 1; k <= n; k++) {
        i64 ans = suf[2 * n - k] - pre[k - 1] - (pre2[2 * n - k] - pre2[2 * n - k - 1] - (pre2[k] - pre2[k - 1]));
        cout << ans << ' ';
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    // t = 1;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    sort(a.begin(), a.end());

    vector<i64> pre(n);
    pre[0] = a[0];
    for (int i = 1; i < n; i++) pre[i] = pre[i - 1] + a[i];

    i64 ans = pre[n - 1];

    auto query = [&](int l, int r) -> i64 {
        assert(l >= 0);
        assert(r < n);
        i64 res = pre[r];
        if (l > 0)
            res -= pre[l - 1];
        return res;
    };

    for (int i = 0; i < n; i++) {
        int l = 0, r = min(i, n - i - 1) + 1;
        while (l + 1 < r) {
            int mid = (l + r) / 2;
            int left = min(1LL * (mid - 1) * k, 1LL * i);
            int right = min(1LL * mid * k - 1, 1LL * i);
            i64 cost = query(left, right) + a[i + mid];
            (cost <= 1ll * a[i] * (right - left + 1 + 1) ? l : r) = mid;
        }
        if (l == 0)
            continue;

        int right = min(1LL * l * k - 1, 1LL * i);
        i64 len = right + 1 + l;
        i64 cost = len * a[i] - (query(0, right) + query(i + 1, i + l));
        ans = max(ans, pre[n - 1] + cost);
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

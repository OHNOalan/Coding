#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

void solve() {
    int n, x;
    i64 k;
    cin >> n >> k >> x, --x;
    vector<i64> h(n), d(n - 1);
    for (auto &x: h) cin >> x;
    for (auto &x: d) cin >> x;
    vector<i64> t(n);
    for (int i = x; i < n - 1; ++i) {
        t[i + 1] = max(t[i] + 1, d[i]);
    }
    for (int i = x - 1; i >= 0; --i) {
        t[i] = max(t[i + 1] + 1, d[i]);
    }
    t[x] = 1;

    vector<int> nh(n), lh(n);
    vector<pair<i64, i64>> stk;
    stk.reserve(n);
    stk.emplace_back(n, 2e9);
    for (int i = n - 1; i >= 0; --i) {
        while (stk.back().second <= h[i]) stk.pop_back();
        nh[i] = stk.back().first;
        stk.emplace_back(i, h[i]);
    }
    stk.clear();
    stk.emplace_back(-1, 2e9);
    for (int i = 0; i < n; ++i) {
        while (stk.back().second <= h[i]) stk.pop_back();
        lh[i] = stk.back().first;
        stk.emplace_back(i, h[i]);
    }

    vector<i64> dp(n);
    vector<pair<i64, i64>> vals;
    for (int i = 0; i < n; ++i) vals.emplace_back(make_pair(t[i], i));
    ranges::sort(vals);
    i64 ans = 0;
    vector<i64> ps(n + 1);
    for (int i = 0; i < n; ++i) {
        ps[i + 1] = ps[i] + h[i];
    }
    for (auto xx: vals) {
        int i = xx.second;
        if (dp[i] < 0) continue;
        ans = max(ans, (k - t[i] + 1) * h[i] + dp[i]);
        for (int j: {nh[i], lh[i]}) {
            if (j < 0 || j >= n) continue;
            if (abs(j - i) > t[j] - t[i]) {
                continue;
            }
            dp[j] = max(dp[j], dp[i] + (t[j] - t[i] - abs(j - i) + 1) * h[i] + ps[max(i, j)] - ps[min(i, j) + 1]);
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

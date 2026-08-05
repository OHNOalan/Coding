#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    ranges::sort(a);
    long long x = a.back();
    a.pop_back();
    n -= 1;
    if (n == 0) return void(cout << x << '\n');
    vector<pair<int, long long>> dp(1 << n);
    auto check = [&](long long m) -> bool {
        fill(dp.begin(), dp.end(), pair<int, long long>(0, 0));
        for (int sub = 0; sub < (1 << n); sub++) {
            auto [cur, sum] = dp[sub];
            for (int i = 0; i < n; i++) {
                if (~sub >> i & 1) {
                    int mask = sub | 1 << i;
                    int ncur = cur;
                    long long nsum = sum + a[i];
                    if (nsum >= m) {
                        ++ncur;
                        nsum = 0;
                    }
                    dp[mask] = max(dp[mask], make_pair(ncur, nsum));
                }
            }
        }
        return dp.back().first >= k;
    };
    long long l = 0, r = accumulate(a.begin(), a.end(), 0ll) / k + 1;
    while (l + 1 < r) {
        long long m = (l + r) / 2;
        (check(m) ? l : r) = m;
    }
    cout << l + x << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

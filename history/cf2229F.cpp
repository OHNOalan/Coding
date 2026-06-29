#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

namespace rgs = std::ranges;

void solve() {
    int n, k;
    std::cin >> n >> k;

    std::vector<int> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    rgs::sort(a);

    n--;
    int last = a[n];
    a.resize(n);

    auto check = [&](i64 x) -> bool {
        std::vector<std::pair<int, i64>> dp(1 << n);
        for (int s = 0; s < (1 << n); ++s) {
            auto [cur, sum] = dp[s];
            for (int i = 0; i < n; i++) {
                if (~s >> i & 1) {
                    int t = s | 1 << i;
                    int ncur = cur;
                    i64 nsum = sum + a[i];
                    if (nsum >= x) {
                        ++ncur;
                        nsum = 0;
                    }
                    dp[t] = std::max(dp[t], std::make_pair(ncur, nsum));
                }
            }
        }
        return dp.back().first >= k;
    };

    i64 lo = 0, hi = 2E10;

    while (lo < hi) {
        i64 x = (lo + hi + 1) / 2;

        if (check(x)) {
            lo = x;
        } else {
            hi = x - 1;
        }
    }

    std::cout << lo + last << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}

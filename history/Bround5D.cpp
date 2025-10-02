#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    ranges::sort(a);
    ll m = a[n / 2];
    ll sum = reduce(a.begin(), a.end(), 0LL);
    auto check = [&](ll k) -> bool {
        if (sum > n * k) return false;
        ll ops = k * n - sum;
        for (int i = n / 2; i < n; i++) {
            ops -= max(0LL, k - a[i]);
        }
        return ops >= 0;
    };
    ll left = m - 1, right = a[n - 1];
    while (left + 1 < right) {
        ll mid = (left + right) / 2;
        (check(mid) ? right : left) = mid;
    }
    cout << right * n - sum << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

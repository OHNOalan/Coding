#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

#define YES cout << "YES\n"
#define NO cout << "NO\n"

void solve() {
    int n;
    cin >> n;
    vector<i64> a(n);
    for (auto &x: a) cin >> x;
    vector<i64> sum(n + 1);
    for (int i = 0; i < n; i++) sum[i + 1] = sum[i] + a[i];
    vector<i64> exact(n + 1);
    exact[0] = exact[n] = 1;
    for (int i = 1; i < n; i++) {
        if (sum[i] * (n - i) < (sum[n] - sum[i]) * i) {
            YES;
            return;
        }
        if (sum[i] * (n - i) == (sum[n] - sum[i]) * i) {
            exact[i] = 1;
        }
    }
    for (int i = 0; i < n; i++) {
        if (!exact[i] && !exact[i + 1]) {
            NO;
            return;
        }
    }
    YES;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

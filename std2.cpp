#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

const i64 INF = LLONG_MAX / 2;


// j1 * (s_1,1 + s_1,j1) + ... + jk * (s_k,1 + s_k,jk)
// \sum_{i=1}^k f(s_i)
//

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    ranges::sort(a);
    vector<i64> pre(n + 1);
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + a[i];
    vector f(n + 1, vector<i64>(k + 1, INF));
    f[0][0] = 0;
    for (int i = 1; i <= n; i++) {
        int x = a[i - 1];
        for (int j = 1; j <= k; j++) {
            for (int k = 0; k < i; k++) {
                f[i][j] = min(f[i][j], f[k][j - 1] + (i - k) * (pre[i] - pre[k]));
            }
        }
    }
    cout << f[n][k] << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

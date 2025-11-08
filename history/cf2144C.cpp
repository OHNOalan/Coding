#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int MOD = 998244353;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int &x: a) cin >> x;
    for (int &x: b) cin >> x;
    int ans = 2;
    for (int i = 0; i < n - 1; i++)
        if (max(a[i], b[i]) <= min(a[i + 1], b[i + 1])) ans = ans * 2 % MOD;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

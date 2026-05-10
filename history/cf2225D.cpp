#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const i64 MOD = 998'244'353;

i64 get0(i64 x) { return 1ll + (x + 1) / 4; }

i64 get1(i64 x) { return (x + 3) / 4; }


void solve() {
    i64 n, x;
    cin >> n >> x;

    i64 ans = 0;
    i64 l0 = get0(x - 1) % MOD;
    ans += l0 * (get0(n) - l0) % MOD;
    i64 l1 = get1(x - 1) % MOD;
    ans += l1 * (get1(n) - l1) % MOD;
    ans %= MOD;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

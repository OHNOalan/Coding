#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
const int INF = 1e9;
const int N = 1e6 + 1;
const int MOD = 998244353;
i64 F[N], INVF[N];

i64 qmul(i64 a, i64 b) {
    i64 res = 1;
    while (b) {
        if (b & 1)
            res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

void init() {
    F[0] = 1;
    for (int i = 1; i < N; i++) { F[i] = F[i - 1] * i % MOD; }
    INVF[N - 1] = qmul(F[N - 1], MOD - 2);
    for (int i = N - 1; i > 0; i--) { INVF[i - 1] = INVF[i] * i % MOD; }
    assert(INVF[0] == 1);
    assert(INVF[1] == 1);
}

i64 C(int n, int r) {
    if (r < 0 || r > n)
        return 0;
    return F[n] * INVF[r] % MOD * INVF[n - r] % MOD;
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    i64 ans = 0;
    for (int k = 1; k < n; k++) {
        int tot = 1;
        int space = 0;
        int c1 = 0, c2 = k;
        for (int x: a) {
            if (x == -1) {
                space++;
                continue;
            }
            if (x > k) {
                int need = x - c2 - 1;
                tot = tot * C(space, need) % MOD;
                c1 += space - need;
                c2 = x;
            } else {
                int need = x - c1 - 1;
                tot = tot * C(space, need) % MOD;
                c2 += space - need;
                c1 = x;
            }
            space = 0;
        }
        assert(space == (k - c1) + (n - c2));
        tot = tot * C(space, k - c1) % MOD;
        ans += tot;
    }
    {
        bool sorted = true;
        for (int i = 0; i < n; i++) {
            if (a[i] != i + 1 && a[i] != -1)
                sorted = false;
        }
        if (sorted)
            ans -= n - 2;
    }
    ans %= MOD;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

#define pb emplace_back
#define mp make_pair
#define x first
#define y second
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

typedef long double ld;
typedef long long ll;

using namespace std;

mt19937 rnd(time(nullptr));

const int inf = 1e9;
const int M = 1e9 + 7;
const ld pi = atan2(0, -1);
const ld eps = 1e-6;
const int M2 = 998244353;
const int MAXN = 2e5;

ll F[MAXN + 1], INVF[MAXN + 1];

ll qmul(ll a, ll b) {
    ll res = 1;
    for (; b; b >>= 1) {
        if (b & 1)
            res = res * a % M2;
        a = a * a % M2;
    }
    return res;
}
int init = []() -> int {
    F[0] = 1;
    for (int i = 1; i <= MAXN; i++) {
        F[i] = F[i - 1] * i % M2;
    }
    INVF[MAXN] = qmul(F[MAXN], M2 - 2);
    for (int i = MAXN; i; i--) {
        INVF[i - 1] = INVF[i] * i % M2;
    }
    return 0;
}();
void solve() {
    int n;
    cin >> n;
    int k = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        k += x & 1;
    }

    ll ans = 1 << (k & 1); // kCk/2 / 2^k
    ans = ans * qmul(INVF[2], k);
    ans = ans * F[k] % M2 * INVF[k / 2] % M2 * INVF[(k + 1) / 2] % M2;
    cout << ans << '\n';
}


#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
        // cout << "\n";
    }
    return 0;
}

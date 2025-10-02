#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 1e6;

using ll = long long;

static constexpr long long MOD = 1e9 + 7;

template<typename T>
T pow(T a, T b) {
    T ret = 1;
    while (b) {
        if (b & 1) {
            ret = (long long) ret * a % MOD;
        }
        a = (long long) a * a % MOD;
        b >>= 1;
    }
    return ret;
}

template<typename T>
T inv(T a) {
    return pow(a, MOD - 2);
}

void solve() {
    ll n, m, N; cin >> n >> m >> N;

    ll prob = m + n;
    prob *= (m + 1);
    prob *= inv(m + n + 1);
    prob %= MOD;
    cout << prob << '\n';
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

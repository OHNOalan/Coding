#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
// static constexpr int MOD = 1e9 + 7;
static constexpr int MOD = 998244353;

int gcd (int a, int b) {
    return b ? gcd (b, a % b) : a;
}


ll quickMul(ll a, ll b) {
    ll ret = 1;
    while (b) {
        if (b & 1) {
            ret = ret * a % MOD;
        }
        a = a * a % MOD;
        b >>= 1;
    }
    return ret;
}

void solve() {
    ll n, k;
    cin >> n >> k;
    ll p = quickMul(2, n-1) + 1;
    ll q = quickMul(2, n);
    ll d = gcd(p, q);
    p /= d, q /= d;
    q = quickMul(q, MOD-2);
    ll res = quickMul(p * q % MOD, k);
    cout << res << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
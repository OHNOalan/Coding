#include <bits/stdc++.h>
using namespace std;
#define MOD 998244353
#define MOD2 1'000'000'007

using ll = long long;

ll exgcd(ll a, ll b, ll& x, ll& y)
{
    if (b == 0)
    {
        x = 1, y = 0;
        return a;
    }
    ll ret = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return ret;
}
ll Inv(ll a, int mod=MOD)
{
    ll x, y;
    ll d = exgcd(a, mod, x, y);
    return d == 1 ? (x % mod + mod) % mod : -1;
}

int cal(int l, int r) {
    int n = r - l - 1;
    return n & 1 ? 1 : (n / 2) + 1;
}

void solve() {
    int n, q; cin >> n >> q;
    set<int> on_seat;
    on_seat.insert(0);
    on_seat.insert(n+1);
    ll num = cal(0, n+1);
    for(int i = 0; i < q; i++) {
        int k; cin >> k;
        auto p = on_seat.find(k);
        if(p != on_seat.end()) {
            int l = *prev(p);
            int r = *next(p);
            on_seat.erase(p);
            num *= cal(l, r);
            num %= MOD;
            num *= Inv(cal(l, k));
            num %= MOD;
            num *= Inv(cal(k, r));
            num %= MOD;
        } else {
            p = on_seat.insert(k).first;
            int l = *prev(p);
            int r = *next(p);
            num *= Inv(cal(l, r));
            num %= MOD;
            num *= cal(l, k);
            num %= MOD;
            num *= cal(k, r);
            num %= MOD;
        }
        cout << num << '\n';
    }
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
    }
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

const int MAXB = 17;
using ll = long long;

template<typename T>
T pow(T a, T b) {
    T ret = 1;
    while (b) {
        if (b & 1) {
            ret = (long long) ret * a;
        }
        a = (long long) a * a;
        b >>= 1;
    }
    return ret;
}

vector<vector<ll>> p;

void init() {
    p = vector<vector<ll>>(MAXB);
    for(int b = 2; b < MAXB; b++) {
        int i = 0;
        while(pow<ll>(b, i) <= pow<ll>(10, 20) || i <= 20) {
            p[b].push_back(pow<ll>(b, i));
            i += 1;
        }
    }
}

ll check(ll n, ll b) {
    ll ans = 0;
    vector<ll> &pb = p[b];
    for(ll x : pb) {
        ll q = n / x;
        ll r = n % x;
        ll last = q % b;
        ans += last * (r + 1);
        for(int j=0; j<b; j++) {
            if(j < last) ans += j * (q / b + 1) * x;
            else ans += j * (q / b) * x;
        }
    }
    return ans;
}

void solve() {
    ll k, b; cin >> k >> b;
    ll left = 0, right = 1e16 + 1;
    while(left + 1 < right) {
        ll mid = (left + right) / 2;
        (check(mid, b) >= k ? right : left) = mid;
    }
    if(right <= 1e16 && check(right, b) == k) {
        cout << right << '\n';
    } else {
        cout << -1 << '\n';
    }
}

#define MULTICASE true
int main() {
    init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
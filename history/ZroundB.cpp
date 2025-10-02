#include <bits/stdc++.h>
using namespace std;


using ll = long long;
constexpr int mod = 1e9 + 7;
constexpr int inf = 0x3f3f3f3f;
constexpr ll linf = 0x3f3f3f3f3f3f3f3f;

constexpr ll maxa = 100'000, maxp = maxa * maxa;
constexpr ll of = 1LL << 32;

bool check(ll u) {
    for (ll i = (u + maxa - 1) / maxa; i * i <= u && i <= maxa; ++i)
        if (u % i == 0) return true;
    return false;
}

void solve() {
    ll n;
    cin >> n;
    bool e1 = false, e2 = false;
    if (n > 0) {
        for (ll u = n; u <= INT32_MAX; u += mod) {
            e1 = e1 || check(u);
            for (ll v = u + of; v <= maxp; v += of) e2 = e2 || check(v);
        }
    } else {
        for (ll u = n; u >= INT32_MIN; u -= mod) {
            for (ll v = u + of; v <= maxp; v += of) e2 = e2 || check(v);
        }
    }
    if (e1) cout << (e2 ? "Maybe\n" : "No\n");
    else cout << (e2 ? "Yes\n" : "Invalid\n");
}

#define MULTICASE true

int main() {
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

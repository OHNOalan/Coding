#include <bits/stdc++.h>
using namespace std;

static constexpr int MAXN = 5e4 + 10;
using ll = long long;
using pii = pair<int, int>;
static constexpr ll MOD = 998244353;

void init() {}

void solve() {
    int n, q;
    cin >> n >> q;
    while(q--) {
        ll u, v;
        cin >> u >> v;
        if(u > v) swap(u, v);
        ll ans = (1LL << n) - 1, add = 0;
        while(true) {
            if(u == ans || v == ans || (u <= ans / 2 && v > ans / 2)) break;
            if(u > ans / 2) {
                u -= ans / 2;
                v -= ans / 2;
                add += ans / 2;
            }
            ans >>= 1;
        }
        cout << ans + add << '\n';
    }
}

#define MULTICASE false
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

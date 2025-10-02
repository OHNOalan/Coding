#include <bits/stdc++.h>
using namespace std;

using ll = long long;
static constexpr int maxn = 100010;
bitset<maxn> b[46], c;
int sg[maxn], p[maxn], mu[maxn];
static constexpr int inf = 0x3f3f3f3f;

void init() {
    fill(mu, mu + maxn, 1);
    c[1] = true;
    for(int i = 2; i < maxn; i++) {
        if(!p[i]) {
            for(int j=i; j<maxn; j+=i) mu[j] *= -1, p[j] = true;
            for(ll j=1LL*i*i; j<maxn; j+=1LL*i*i) mu[j] = 0;
        }
        c[i] = mu[i] == 1;
    }
    b[0] = c;
    for(int i = 1; i < maxn; i++) {
        c <<= 1;
        while(b[sg[i]][i]) sg[i]++;
        b[sg[i]] |= c;
    }
}

void solve() {
    int m; cin >> m;
    int x = 0;
    for(int i=0; i<m; i++) {
        int u; cin >> u;
        x ^= sg[u];
    }
    cout << (x ? "Alice\n" : "Bob\n");
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

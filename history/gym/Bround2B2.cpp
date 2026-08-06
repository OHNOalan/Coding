#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 10;
int isPrime[MAXN];
int f[MAXN];
int primes[MAXN];
int pos;

void init() {
    pos = 0;
    memset(isPrime, 0, sizeof(isPrime));
    for(int i=2; i<MAXN; i++) {
        if(!isPrime[i]) {
            primes[pos++] = i;
            for(int j=i; j<MAXN; j+=i) {
                isPrime[j] = true;
            }
        }
    }
    memset(f, 0x3f, sizeof(f));
    f[0] = 0;
    for(int i=2; i<MAXN; i++) {
        for(int j=0; j<pos && i - primes[j] >= 0; j++) {
            f[i] = min(f[i], f[i-primes[j]] + 1);
        }
    }
}

void solve() {
    int n; cin >> n;
    cout << f[n] << '\n';
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
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
constexpr int MAXN = 50;

int comp[MAXN + 1];
vector<ll> primes;

void init() {
    primes.push_back(2);
    for(int i = 2; i<=MAXN; i++) {
        for(ll j = (1LL << i) + 1; j <= 1LL << (i+1); j++) {
            bool flag = true;
            for(ll x : primes) {
                if(j % x == 0) {
                    flag = false;
                    break;
                }
            }
            if(flag) {
                primes.push_back(j);
                break;
            }
        }
    }
}
void solve() {
    int n; cin >> n;
    for(int i=0; i<n; i++) cout << primes[i] << " "; cout << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
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

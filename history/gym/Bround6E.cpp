#include <bits/stdc++.h>
using namespace std;

using ll = long long;
constexpr int MAXN = 300;


int comp[MAXN + 1];
vector<int> primes;

void init() {
    memset(comp, 0, sizeof(comp));
    for(int i=2; i<=MAXN; i++) if(!comp[i]) {
        for(ll j = 1LL * i*i; j<=MAXN; j+=i) {
            comp[j] = true;
        }
        primes.push_back(i);
    }
}
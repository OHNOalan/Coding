#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int gcd (int a, int b) {
    return b ? gcd (b, a % b) : a;
}

void solve() {
    int N, P;
    cin >> N >> P;
    int q = P + 1;
    int p = (ll) q * N % P;
    int d = gcd(p, q);
    p /= d, q /= d;
    cout << p << ' ' << q << endl;
}

#define MULTICASE true

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

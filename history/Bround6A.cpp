#include <bits/stdc++.h>
using namespace std;

using ll = long long;

constexpr int f[10] = {1, 0, 0, 0, 1, 0, 1, 0, 2, 1};

void solve() {
    int m, k; cin >> m >> k;
    while(k && m >= 2) {
        int nm = 0;
        while(m) {
            nm += f[m % 10];
            m /= 10;
        }
        m = nm;
        k--;
    }
    if(k & 1) m = 1 - m;
    cout << m << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
     while (t--) {
         solve();
     }
    return 0;
}

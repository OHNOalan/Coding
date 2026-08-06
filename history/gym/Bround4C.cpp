#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2010;
using ll = long long;

void solve() {
    int n, k; cin >> n >> k;
    map<int,int> cnt;
    for(int i=0; i<n; i++) {
        int x; cin >> x;
        x %= k;
        if(x) cnt[k-x]++;
    }

    int mx = 0, idx = 0;
    for(auto [id, x] : cnt) {
        if(x >= mx) {
            mx = x;
            idx = id;
        }
    }
    cout << (ll) k * (mx - 1) + idx + 1 << '\n';
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
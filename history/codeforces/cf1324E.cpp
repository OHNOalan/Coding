#include <bits/stdc++.h>
using namespace std;


using ll = long long;

void solve() {
    int n, h, l, r; cin >> n >> h >> l >> r;
    vector<int> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];
    vector<int> dp(h, -1);
    dp[0] = 0;
    for(int i = 0; i < n; i++) {
        int x = a[i];
        vector<int> ndp(h, -1);
        for(int t=0; t<h; t++) if(dp[t] >= 0) {
            int nt = (t+x) % h;
            int good = (nt >= l && nt <= r);
            ndp[nt] = max(ndp[nt], dp[t] + good);
            nt = (t+x-1) % h;
            good = (nt >= l && nt <= r);
            ndp[nt] = max(ndp[nt], dp[t] + good);
        }
        dp = move(ndp);
    }
    int mx = *max_element(dp.begin(), dp.end());
    cout << mx << '\n';
}

#define MULTICASE false

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

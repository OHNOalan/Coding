#include <bits/stdc++.h>

using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> dp(n);
    vector<ll> save_one(n);
    for (int v = n - 1; v >= 0; v--) {
        ll a;
        cin >> a;
        int k;
        cin >> k;
        dp[v] = 0;
        save_one[v] = 1;
        for (int i = 0; i < k; i++) {
            int c;
            cin >> c;
            c--;
            assert(c > v);
            dp[v] += dp[c];
            save_one[v] = lcm(save_one[v], gcd(a, save_one[c]));
        }
        if (save_one[v] == 1) {
            save_one[v] = a;
            dp[v] += 1;
        }
        cout << dp[v] << '\n';
        cout << flush;
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

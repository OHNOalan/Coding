#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int MOD = 1'000'000'007;

void solve() {
    int n;
    cin >> n;
    vector<int> b(n), a(n);
    for (int &x: b) cin >> x;
    for (int i = 0; i < n; i++) {
        a[i] = 1;
        for (int j = 0; j < i; j++)
            if (b[j] <= b[i]) a[i]++;
        for (int j = i + 1; j < n; j++)
            if (b[j] < b[i]) a[i]++;
    }
    vector dp(2, vector(n + 1, vector<int>(n + 1)));
    dp[0][0][0] = 1;
    int cr = 0;
    for (int i = 0; i < n; i++) {
        cr ^= 1;
        int x = a[i];
        for (int j = 0; j <= n; j++) {
            for (int q = 0; q <= n; q++) {
                dp[cr][j][q] = dp[cr ^ 1][j][q];
            }
        }
        for (int j = 0; j <= n; j++) {
            for (int q = 0; q <= j; q++) {
                if (dp[cr ^ 1][j][q] == 0) continue;
                if (j > x and x > q)
                    dp[cr][j][x] = (dp[cr][j][x] + dp[cr ^ 1][j][q]) % MOD;
                else if (x > j)
                    dp[cr][x][q] = (dp[cr][x][q] + dp[cr ^ 1][j][q]) % MOD;
            }
        }
    }
    int ans = 0;
    for (int j = 0; j <= n; j++) {
        for (int q = 0; q <= n; q++) ans = (ans + dp[cr][j][q]) % MOD;
    }
    cout << ans << '\n';
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

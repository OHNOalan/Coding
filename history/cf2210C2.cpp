#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

vector<ll> pri = {1, 2};
int L = pri.size();

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 2), b(n + 2);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];
    a[0] = a[n + 1] = 1;

    vector<int> c(n + 2);
    for (int i = 1; i <= n; i++) {
        c[i] = lcm(gcd(a[i], a[i - 1]), gcd(a[i], a[i + 1]));
        if (c[i] > b[i]) c[i] = a[i];
    }
    c[0] = c[n + 1] = 1;

    vector<vector<int>> dp(n + 1, vector<int>(L, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < L; j++) {
            dp[i][j] = -1;
            for (int l = 0; l < L; l++) {
                if (gcd(c[i] * pri[j], c[i - 1] * pri[l]) == gcd(c[i], c[i - 1]))
                    dp[i][j] = max(dp[i][j], dp[i - 1][l]);
            }
            if (c[i] * pri[j] != a[i] && c[i] * pri[j] <= b[i]) dp[i][j]++;
        }
    }
    cout << ranges::max(dp[n]) << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<string> ss(n);
    for (auto &s: ss) cin >> s;
    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) a[i][j] = ss[i][j] & 1;

    bool trans = false;
    if (n > m) {
        trans = true;
        vector<vector<int>> b(m, vector<int>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) b[j][i] = a[i][j];
        swap(n, m);
        a = std::move(b);
    }

    vector<vector<int>> dp(n, vector<int>(n, -1));
    vector<vector<vector<int>>> mns(n, vector<vector<int>>(m, vector<int>(n, INT_MAX)));
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            if (a[i][j] == 1) {
                for (int k = i + 1; k < n; k++) {
                    if (a[k][j] == 1) {
                        if (dp[i][k] != -1) {
                            // find all unique Rectangular
                            int area = (k - i + 1) * (j - dp[i][k] + 1);
                            for (int l = j; l >= dp[i][k]; l--) { mns[i][l][k] = min(mns[i][l][k], area); }
                        }
                    }
                }

                for (int k = i + 1; k < n; k++) {
                    if (a[k][j] == 1) {
                        dp[i][k] = j;
                    }
                }
            }
        }
    }

    vector<vector<int>> ans(n, vector<int>(m, INT_MAX));
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            int mn = INT_MAX;
            for (int k = n - 1; k >= i; k--) {
                mn = min(mn, mns[i][j][k]);
                ans[k][j] = min(ans[k][j], mn);
            }
        }
    }

    if (trans) {
        for (int j = 0; j < m; j++)
            for (int i = 0; i < n; i++) {
                { cout << (ans[i][j] != INT_MAX ? ans[i][j] : 0) << " "; }
                cout << '\n';
            }
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) { cout << (ans[i][j] != INT_MAX ? ans[i][j] : 0) << " "; }
            cout << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

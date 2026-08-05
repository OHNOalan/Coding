#include <bits/stdc++.h>

using namespace std;

constexpr int mod = 998244353;

void solve() {
    int n;
    string s;
    cin >> n >> s;
    vector<int> next(n + 1);
    next[n] = n;
    for (int i = n - 1; i >= 0; i--) next[i] = s[i] == '?' ? i : next[i + 1];
    vector<int> pre(n + 1);
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] ^ (s[i] == '1');
    vector<int> end(n + 1);
    for (int i = 0; i <= n; i++) end[i] = next[i] < n || pre[n] == pre[i];

    vector trans(n + 1, array<array<int, 2>, 2>{});
    for (int i = 0; i <= n; i++) {
        trans[i][0] = trans[i][1] = {-1, -1};
        for (int c = 0; c < 2; c++) {
            for (int j = i; j < n; j++) {
                if (s[j] == '?' || s[j] == '0' + c) {
                    if (next[i] < j || pre[i] == pre[j]) {
                        if (trans[i][c][0] == -1)
                            trans[i][c][0] = j + 1;
                        else {
                            int k = trans[i][c][0];
                            if (next[k] >= j + 1 && pre[j + 1] != pre[k]) {
                                trans[i][c][1] = j + 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    vector dp(n + 1, vector<int>(n + 1));
    dp[0][0] = 1;
    int ans = 0;
    for (int i = 0; i <= n; i++) {
        for (int j = i; j <= n; j++) {
            if (dp[i][j] == 0) continue;
            if (end[i] || end[j]) ans = (ans + dp[i][j]) % mod;
            for (int c = 0; c < 2; c++) {
                int a[4] = {trans[i][c][0], trans[i][c][1], trans[j][c][0], trans[j][c][1]};
                ranges::sort(a, a + 4);
                int x = -1, y = -1;
                for (auto z: a) {
                    if (z == -1) continue;
                    if (x == -1) {
                        x = z;
                    } else if (next[x] >= z && pre[x] != pre[z]) {
                        y = z;
                        break;
                    }
                }
                if (y != -1) {
                    dp[x][y] = (dp[x][y] + dp[i][j]) % mod;
                } else if (x != -1) {
                    dp[x][x] = (dp[x][x] + dp[i][j]) % mod;
                }
            }
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

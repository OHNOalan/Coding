#include <bits/stdc++.h>

using namespace std;

constexpr int mod = 1'000'000'007;

void solve() {
    int n;
    string s;
    cin >> n >> s;
    s = '1' + s;
    vector<vector<int>> line(n);
    for (int i = 0, c; i < n; i++) {
        cin >> c;
        for (int j = 0, x; j < c; j++) {
            cin >> x, --x;
            line[i].push_back(x);
        }
    }

    vector dp(4, vector(n, vector<int>(n + 1)));
    vector dpc(4, vector(n, vector<int>(n + 1)));
    vector tdp(4, vector<int>(n + 1));
    vector tdpc(4, vector<int>(n + 1));

    vector<int> dep(n), vislink(n), vis1(n);

    auto merge = [&](int u, int v, int pu, int pv, int pt) -> void {
        for (int i = 0; i <= dep[u]; i++) {
            for (int j = 0; j <= dep[v]; j++) {
                tdp[pt][max(i, j)] = (tdp[pt][max(i, j)] + 1ll * dp[pu][u][i] * dp[pv][v][j]) % mod;
                if (i <= j)
                    tdpc[pt][j] = (tdpc[pt][j] + 1ll * dp[pu][u][i] * dpc[pv][v][j]) % mod;
                else
                    tdpc[pt][i] = (tdpc[pt][i] + 1ll * dpc[pu][u][i] * dp[pv][v][j]) % mod;
            }
        }
    };

    [&](this auto &&dfs, int t) -> void {
        int r0 = 0;
        dp[0][t][0] = 1;
        dep[t] = 0;
        vislink[t] = 0;
        vis1[t] = 0;
        for (int it: line[t]) r0 += s[it] == '0';
        for (int it: line[t]) {
            dfs(it);
            r0 -= s[it] == '0';
            merge(t, it, 0, 0, 0);
            merge(t, it, 3, 1, 3);
            if (s[it] == '1')
                merge(t, it, 3, 1, 2);
            else
                merge(t, it, 2, 1, 2);
            if (vis1[t] == 0 && r0 == 0 && vislink[it]) {
                if (vislink[t] == 0) {
                    vislink[t] = 1;
                    for (int i = 0; i <= dep[it]; i++) {
                        tdp[2][i] = (tdp[2][i] + dp[2][it][i]) % mod;
                        tdpc[2][i] = (tdpc[2][i] + dpc[2][it][i]) % mod;
                        tdp[3][i] = (tdp[3][i] + dp[3][it][i]) % mod;
                        tdpc[3][i] = (tdpc[3][i] + dpc[3][it][i]) % mod;
                    }
                } else {
                    for (int i = 0; i <= dep[it]; i++) {
                        if (s[it] == '1') {
                            tdp[2][i] = (tdp[2][i] + dp[1][it][i]) % mod;
                            tdpc[2][i] = (tdpc[2][i] + dpc[1][it][i]) % mod;
                            tdp[3][i] = (tdp[3][i] + dp[1][it][i]) % mod;
                            tdpc[3][i] = (tdpc[3][i] + dpc[1][it][i]) % mod;
                        } else {
                            tdp[2][i] = (tdp[2][i] + dp[2][it][i]) % mod;
                            tdpc[2][i] = (tdpc[2][i] + dpc[2][it][i]) % mod;
                            tdp[3][i] = (tdp[3][i] + dp[2][it][i]) % mod;
                            tdpc[3][i] = (tdpc[3][i] + dpc[2][it][i]) % mod;
                        }
                    }
                }
            }
            vis1[t] |= vis1[it];
            dep[t] = max(dep[t], dep[it]);
            for (int p = 0; p < 4; p++) {
                for (int i = 0; i <= dep[t]; i++) {
                    dp[p][t][i] = exchange(tdp[p][i], 0);
                    dpc[p][t][i] = exchange(tdpc[p][i], 0);
                }
            }
        }

        if (s[t] == '0') return;
        for (int i = 0; i <= dep[t]; i++) {
            tdp[0][i + 1] = (tdp[0][i + 1] + dp[0][t][i]) % mod;
            tdpc[0][i + 1] = (tdpc[0][i + 1] + dpc[0][t][i]) % mod;
        }

        int vl = 0;
        for (int i = 0; i <= dep[t]; i++) {
            vl = (vl + dp[2][t][i]) % mod;
            vl = (vl + mod - dpc[2][t][i]) % mod;
        }

        tdpc[0][1] = (tdpc[0][1] + vl) % mod;
        dep[t]++;
        vislink[t] = 1;

        if (s[t] == '1') {
            vis1[t] = 1;
            for (int i = 0; i <= dep[t]; i++) {
                dp[0][t][i] = dp[1][t][i] = tdp[0][i];
                dpc[0][t][i] = dpc[1][t][i] = tdpc[0][i];
                dp[2][t][i] = dpc[2][t][i] = 0;
                dp[3][t][i] = dpc[3][t][i] = 0;
            }
        } else {
            for (int i = 0; i <= dep[t]; i++) {
                dp[0][t][i] = (dp[0][t][i] + tdp[0][i]) % mod;
                dpc[0][t][i] = (dpc[0][t][i] + tdpc[0][i]) % mod;
                dp[1][t][i] = tdp[0][i];
                dpc[1][t][i] = tdpc[0][i];

                dp[3][t][i] = dp[2][t][i];
                dpc[3][t][i] = dpc[2][t][i];
                dp[2][t][i] = (dp[2][t][i] + tdp[0][i]) % mod;
                dpc[2][t][i] = (dpc[2][t][i] + tdpc[0][i]) % mod;
            }
        }

        fill(tdp[0].begin(), tdp[0].begin() + dep[t] + 1, 0);
        fill(tdpc[0].begin(), tdpc[0].begin() + dep[t] + 1, 0);
    }(0);

    long long ans = 0;
    for (int i = 0; i <= dep[0]; i++) {
        ans = (ans + dp[0][0][i]) % mod;
        ans = (ans + mod - dpc[0][0][i]) % mod;
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

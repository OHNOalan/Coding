#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<long long> w(n);
    for (int i = 0; i < n; i++) cin >> w[i];
    vector<vector<int>> g(n * 2);
    for (int i = 0, u, v; i < n - 1; i++) {
        cin >> u >> v, --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<int> sz(n);
    vector<vector<int>> ch(n);
    vector down(n, vector<long long>(k + 1));
    vector take(n, vector<long long>(k + 1));
    vector up(n, vector<long long>(k + 1));
    [&](this auto &&self, int u, int p) -> void {
        sz[u] = 1;
        for (int v: g[u]) {
            if (v == p) continue;
            ch[u].push_back(v);
            self(v, u);
            for (int i = min(sz[u] - 1, k); i >= 0; --i) {
                for (int j = 1; j <= sz[v] && i + j <= k; ++j) {
                    down[u][i + j] = max(down[u][i + j], down[u][i] + take[v][j]);
                }
            }
            sz[u] += sz[v];
        }
        for (int i = min(sz[u], k); i >= 1; --i) {
            take[u][i] = max(down[u][i], down[u][i - 1] + i * w[u]);
        }
    }(0, -1);

    vector pre(n, vector<long long>(k + 1));
    vector suf(n, vector<long long>(k + 1));
    [&](this auto &&self, int u) -> void {
        int deg = int(ch[u].size());
        if (deg == 1) {
            int v = ch[u][0];
            for (int i = 0; i <= k; ++i) {
                up[v][i] = up[u][i];
            }
        } else if (deg >= 2) {
            int p = sz[ch[u][0]];
            for (int i = 0; i <= k; ++i) {
                pre[0][i] = take[ch[u][0]][i];
                suf[deg][i] = up[u][i];
            }

            for (int c = 1; c < deg; ++c) {
                for (int i = 0; i <= k; ++i) {
                    pre[c][i] = 0;
                }
                int v = ch[u][c];
                p += sz[v];
                for (int i = min(k, p); i >= 0; --i) {
                    for (int j = min(i, sz[v]); j >= 0 && i - j <= p - sz[v]; --j) {
                        pre[c][i] = max(pre[c][i], pre[c - 1][i - j] + take[v][j]);
                    }
                }
            }

            for (int c = deg - 1; c >= 0; --c) {
                for (int i = 0; i <= k; ++i) {
                    suf[c][i] = 0;
                }
                int v = ch[u][c];
                p -= sz[v];

                for (int i = k; i >= max(0, k - p - 1); --i) {
                    for (int j = min(i, sz[v]); j >= 0; --j) {
                        suf[c][i] = max(suf[c][i], suf[c + 1][i - j] + take[v][j]);
                    }
                }

                for (int i = k; i >= max(0, k - sz[v] - 1); --i) {
                    for (int j = min(i, p); j >= 0; --j) {
                        up[v][i] = max(up[v][i], suf[c + 1][i - j] + (c > 0 ? pre[c - 1][j] : 0));
                    }
                }
            }
        }
        for (auto v: ch[u]) {
            for (int i = min(k, n - sz[v]); i >= 1; --i) {
                up[v][i] = max(up[v][i], up[v][i - 1] + i * w[u]);
            }
            self(v);
        }
    }(0);


    for (int x = 0; x < n; x++) {
        long long mx = 0;
        for (int j = 0; j < min(k, sz[x]); j++) {
            mx = max(mx, down[x][j] + up[x][k - 1 - j]);
        }
        cout << mx + k * w[x] << " ";
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

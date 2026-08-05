#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<long long> w(n * 2);
    for (int i = 0; i < n; i++) cin >> w[i];
    vector<vector<int>> g(n * 2);
    for (int i = 0, u, v; i < n - 1; i++) {
        cin >> u >> v, --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<vector<int>> G(2 * n);
    int N = n;
    [&](this auto &&self, int u, int p) -> void {
        vector<int> kids;
        for (int v: g[u]) {
            if (v != p) kids.push_back(v);
        }
        int z = u;
        for (int i = 0; i < (int) kids.size(); i++) {
            int v = kids[i];
            if (i > 0 && i < (int) kids.size() - 1) {
                int nz = N++;
                w[nz] = w[u];
                G[z].push_back(nz);
                z = nz;
            }
            G[z].push_back(v);
            self(v, u);
        }
    }(0, -1);

    vector<int> sz(N), fa(N, -1);
    [&](this auto &&self, int u) -> void {
        sz[u] = (u < n ? 1 : 0);
        for (int v: G[u]) {
            fa[v] = u;
            self(v);
            sz[u] += sz[v];
        }
    }(0);

    auto merge = [&](const vector<long long> &u_in, const vector<long long> &v_in, vector<long long> &t,
                     int target_sz) -> void {
        int tsize = (int) t.size() - 1;
        int topmost = min(tsize + 1, target_sz + 2);
        int bottom = max(0, tsize - topmost);

        for (int i = tsize; i >= bottom; i--) {
            int j_max = min({(int) u_in.size(), (int) v_in.size(), i + 1});
            for (int j = 0; j < j_max; j++) {
                if (i - j < (int) u_in.size()) t[i] = max(t[i], u_in[i - j] + v_in[j]);
                if (i - j < (int) v_in.size()) t[i] = max(t[i], u_in[j] + v_in[i - j]);
            }
        }
    };

    // dp1 (include or not include) u within subtree
    // dp2 not include u within subtree
    vector dp1(N, vector<long long>(1));
    vector dp2(N, vector<long long>());
    [&](this auto &&self, int u, int p) -> void {
        fa[u] = p;
        int cnt = 0;
        for (int v: G[u]) {
            self(v, u);
            vector<long long> dp1t(min(k + 1, cnt + sz[v] + 1), 0);
            for (int i = 0; i < (int) dp1[u].size(); i++) {
                for (int j = 0; j < (int) dp1[v].size(); j++) {
                    if (i + j < (int) dp1t.size()) {
                        dp1t[i + j] = max(dp1t[i + j], dp1[u][i] + dp1[v][j]);
                    }
                }
            }
            dp1[u] = std::move(dp1t);
            cnt += sz[v];
        }
        if (u < n) {
            dp2[u] = dp1[u];
            if ((int) dp1[u].size() <= k) dp1[u].push_back(0);
            for (int x = (int) dp1[u].size() - 1; x > 0; x--) {
                dp1[u][x] = max(dp1[u][x], dp1[u][x - 1] + x * w[u]);
            }
        }
    }(0, -1);

    // dp3 outside the subtree
    vector dp3(N, vector<long long>(1));
    [&](this auto &&self, int u, int p) -> void {
        for (int i = 0; i < (int) G[u].size(); i++) {
            int v = G[u][i];
            if ((int) G[u].size() == 2) {
                int z = G[u][i ^ 1]; // z is the sibling of v
                int new_sz = min(k + 1, (int) dp1[z].size() + (int) dp3[u].size() - 1);
                dp3[v].assign(new_sz, 0);
                merge(dp1[z], dp3[u], dp3[v], sz[v]);
            } else {
                dp3[v] = dp3[u];
            }
            if (v < n) {
                if ((int) dp3[v].size() <= k) dp3[v].push_back(0);
                for (int x = (int) dp3[v].size() - 1; x >= 1; x--) {
                    dp3[v][x] = max(dp3[v][x], dp3[v][x - 1] + x * w[u]);
                }
            }
            self(v, u);
        }
    }(0, -1);
    for (int i = 0; i < n; i++) {
        long long mx = 0;
        for (int x = 0; x < min(k, (int) dp2[i].size()); x++) {
            int y = (k - 1) - x;
            if (y < (int) dp3[i].size()) {
                mx = max(mx, dp2[i][x] + dp3[i][y]);
            }
        }
        cout << mx + k * w[i] << " ";
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

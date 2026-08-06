#include <bits/stdc++.h>

using namespace std;

constexpr long long MOD = 998244353;

long long qpow(long long a, long long b) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

const int MX = 20;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<string> s(n);
    for (int i = 0; i < n; i++) cin >> s[i];

    auto pos = [&](int i, int j) -> int { return i * m + j; };


    vector<vector<int>> g(n * m);
    vector<int> h;
    vector<vector<int>> up(MX, vector<int>(n * m));
    auto newv = [&](int v, int p) -> void {
        g[v].push_back(p);
        h[v] = h[p] + 1;
        up[0][v] = p;
        for (int j = 1; j < MX; j++) up[j][v] = up[j - 1][up[j - 1][v]];
    };

    auto jump = [&](int v, int where) -> int {
        assert(h[v] >= where);
        where = h[v] - where;
        for (int i = MX - 1; i >= 0; i--) {
            if (where >> i & 1) v = up[i][v];
        }
        return v;
    };

    auto lca = [&](int u, int v) -> int {
        if (h[u] < h[v]) swap(u, v);
        u = jump(u, h[v]);
        if (u == v) return u;
        for (int i = MX - 1; i >= 0; i--) {
            int nu = up[i][u], nv = up[i][v];
            if (nu != nv) u = nu, v = nv;
        }
        return up[0][u];
    };

    h.assign(n * m, 0);
    for (int j = 0; j < MX; j++) up[j][0] = 0;
    vector<int> f1(n * m), f2(n * m);
    f1[pos(0, 0)] = f2[pos(n - 1, m - 1)] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i == 0 && j == 0 || s[i][j] == '0') continue;
            int u = 0, v = 0;
            if (i > 0) u = f1[pos(i - 1, j)];
            if (j > 0) v = f1[pos(i, j - 1)];
            f1[pos(i, j)] = u || v;
            if (f1[pos(i, j)]) {
                int par;
                if (!u)
                    par = pos(i, j - 1);
                else if (!v)
                    par = pos(i - 1, j);
                else
                    par = lca(pos(i, j - 1), pos(i - 1, j));
                newv(pos(i, j), par);
            }
        }
    }
    h.assign(n * m, 0);
    for (int j = 0; j < MX; j++) up[j][n * m - 1] = n * m - 1;
    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            if (i == n - 1 && j == m - 1 || s[i][j] == '0') continue;
            int u = 0, v = 0;
            if (i < n - 1) u = f2[pos(i + 1, j)];
            if (j < m - 1) v = f2[pos(i, j + 1)];
            f2[pos(i, j)] = u || v;
            if (f2[pos(i, j)]) {
                int par;
                if (!u)
                    par = pos(i, j + 1);
                else if (!v)
                    par = pos(i + 1, j);
                else
                    par = lca(pos(i, j + 1), pos(i + 1, j));
                newv(pos(i, j), par);
            }
        }
    }
    int c = 0;
    for (int i = 0; i < n * m; i++)
        if (!f1[i] || !f2[i]) c++;
    auto cnt = [](int c) -> long long { return qpow(2, c) - 1; };
    long long ans = cnt(c);

    int timer = 0;
    vector<int> vis(n * m);
    vector<int> low(n * m), dis(n * m);
    vector<int> in_stack(n * m);
    stack<int> stk;
    stack<pair<int, int>> call_stk;
    for (int i = 0; i < n * m; i++) {
        if (!vis[i] && f1[i] && f2[i]) {
            stk.push(i);
            call_stk.push({i, 0});
            dis[i] = low[i] = ++timer;
            vis[i] = true;
            in_stack[i] = true;
            while (!call_stk.empty()) {
                auto [u, idx] = call_stk.top();
                if (idx < (int) g[u].size()) {
                    int v = g[u][idx];
                    call_stk.top().second++;
                    if (!vis[v]) {
                        stk.push(v);
                        call_stk.push({v, 0});
                        dis[v] = low[v] = ++timer;
                        vis[v] = true;
                        in_stack[v] = true;
                    } else if (in_stack[v]) {
                        low[u] = min(low[u], dis[v]);
                    }
                } else {
                    call_stk.pop();
                    if (!call_stk.empty()) {
                        int parent = call_stk.top().first;
                        low[parent] = min(low[parent], low[u]);
                    }
                    if (low[u] == dis[u]) {
                        c = 0;
                        int curr;
                        do {
                            curr = stk.top();
                            in_stack[curr] = false;
                            stk.pop();
                            c++;
                        } while (curr != u);
                        ans = (ans + cnt(c)) % MOD;
                    }
                }
            }
        }
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

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

struct DSU {
    vector<int> f, siz;

    DSU() {}
    DSU(int n) { init(n); }

    void init(int n) {
        f.resize(n + 1);
        iota(f.begin(), f.end(), 0);
        siz.assign(n + 1, 1);
    }

    int find(int x) {
        while (x != f[x]) {
            x = f[x] = f[f[x]];
        }
        return x;
    }

    bool same(int x, int y) { return find(x) == find(y); }

    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return true;
        }
        siz[x] += siz[y];
        f[y] = x;
        return false;
    }

    int size(int x) { return siz[find(x)]; }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<string> s(n);
    for (int i = 0; i < n; i++) cin >> s[i];

    auto pos = [&](int i, int j) -> int { return i * m + j; };


    vector<vector<int>> g(n * m);
    vector<int> par1(n * m, -1), par2(n * m, -1);
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
                par1[pos(i, j)] = par;
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
                par2[pos(i, j)] = par;
            }
        }
    }
    int c = 0;
    for (int i = 0; i < n * m; i++)
        if (!f1[i] || !f2[i]) c++;
    auto cnt = [](int c) -> long long { return qpow(2, c) - 1; };
    long long ans = cnt(c);

    DSU dsu(n * m);
    for (int i = 0; i < n * m; i++) {
        if (f1[i] && f2[i]) {
            int p1 = par1[i];
            if (p1 != -1 && f1[p1] && f2[p1]) {
                if (par2[p1] == i) {
                    dsu.merge(i, p1);
                }
            }
        }
    }
    for (int i = 0; i < n * m; i++) {
        if (f1[i] && f2[i] && dsu.find(i) == i) {
            ans = (ans + cnt(dsu.size(i))) % MOD;
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

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
constexpr ll LNF = 0x3f3f3f3f3f3f3f3f;

struct DSU {
    vector<int> f, siz;

    DSU() {}
    DSU(int n) {
        init(n);
    }

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

    bool same(int x, int y) {
        return find(x) == find(y);
    }

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

    int size(int x) {
        return siz[find(x)];
    }
};

struct Edge {
    int u, v, w;

    Edge() {
    }
    Edge(int u, int v, int w) : u(u), v(v), w(w) {
    }
    bool operator<(const Edge& e) const {
        return w < e.w;
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }
    auto bitCount = [](int x) -> int { return __builtin_popcount(x); };
    int full = (1 << m) - 1;
    vector<ll> memo(full + 1, -1);
    auto f = [&](auto&& self, int u) -> ll {
        if(memo[u] != -1)
            return memo[u];
        DSU dsu(n);
        for (int j = 0; j < m; j++) {
            if (u >> j & 1) {
                dsu.merge(edges[j].u, edges[j].v);
            }
        }
        for (int i = 1; i <= n; i++) {
            if (dsu.size(i) == n) {
                return memo[u] = 0;
            }
        }
        ll res = 0;
        if (bitCount(u) & 1) {
            res = 0;
            for (int j = 0; j < m; j++) {
                if (u >> j & 1)
                    continue;
                if (dsu.same(edges[j].u, edges[j].v))
                    continue;
                res = max(res, self(self, u | 1 << j) + edges[j].w);
            }
        } else {
            res = LNF;
            for (int j = 0; j < m; j++) {
                if (u >> j & 1)
                    continue;
                if (dsu.same(edges[j].u, edges[j].v))
                    continue;
                res = min(res, self(self, u | 1 << j) + edges[j].w);
            }
        }
        return memo[u] = res;
    };
    cout << f(f, 0) << '\n';
}

#define MULTICASE false
int main() {
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<int>> g(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> fa(n, -1), sz(n, 1), dep(n), wp(n, -1);
    dep[0] = 0;
    [&](this auto &&self, int u, int f) -> void {
        int mx = -1;
        fa[u] = f;
        for (int v: g[u]) {
            if (v == f) continue;
            dep[v] = dep[u] + 1;
            self(v, u);
            sz[u] += sz[v];
            if (sz[v] > mx) {
                mx = sz[v];
                wp[u] = v;
            }
        }
    }(0, -1);

    int cdfn = 0;
    vector<int> dfn(n), tp(n);
    tp[0] = 0;
    [&](this auto &&self, int u, int f) -> void {
        dfn[u] = cdfn++;
        if (wp[u] == -1) return;
        tp[wp[u]] = tp[u];
        self(wp[u], u);
        for (int v: g[u]) {
            if (v == f || v == wp[u]) continue;
            tp[v] = v;
            self(v, u);
        }
    }(0, -1);

    map<int, int> mp, sg;
    auto access = [&](int p) -> void {
        auto it = --sg.upper_bound(p);
        int x = it->second;
        sg[p] = x;
    };
    auto chkmap = [&](int c, int val) -> int {
        while (true) {
            auto it = mp.upper_bound(val);
            if (it == mp.begin()) break;
            it--;
            int d = min(it->second, c);
            c -= d;
            it->second -= d;
            if (it->second == 0) mp.erase(it);
            if (c == 0) break;
        }
        return c;
    };
    auto upd = [&](int l, int r, int p) -> int {
        access(l);
        access(r);
        int lst = l;
        int lvl = sg[l];
        int ans = 0;
        while (1) {
            auto it = sg.upper_bound(l);
            ans += chkmap(it->first - lst, lvl);
            lst = it->first;
            lvl = it->second;
            if (it->first == r) break;
            sg.erase(it);
        }
        sg[l] = p;
        mp[p] += r - l - ans;
        return ans;
    };
    sg[0] = -1;
    sg[n] = -1;
    int p;
    for (int i = 0; i < m; i++) {
        cin >> p, --p;
        access(dfn[p] + 1);
        sg[dfn[p]] = 0;
    }
    mp[0] = m;
    long long ans = 0;
    int u, v;
    for (int i = 1; i <= q; i++) {
        u = p;
        cin >> p, --p;
        p = (p + ans) % n;
        v = p;
        while (true) {
            if (tp[u] == tp[v]) {
                if (dep[v] > dep[u]) swap(u, v);
                ans += upd(dfn[v], dfn[u] + 1, i);
                break;
            }
            if (dep[tp[v]] > dep[tp[u]]) swap(u, v);
            ans += upd(dfn[tp[u]], dfn[u] + 1, i);
            u = fa[tp[u]];
        }
        cout << ans << ' ';
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

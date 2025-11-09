#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

#define debug(arr)                                                                                                     \
    cerr << #arr << ": ";                                                                                              \
    for (const auto &_x: arr) {                                                                                        \
        cerr << _x << " ";                                                                                             \
    }                                                                                                                  \
    cerr << endl;


namespace std {

    template<class Fun>
    class y_combinator_result {
        Fun fun_;

    public:
        template<class T>
        explicit y_combinator_result(T &&fun) : fun_(std::forward<T>(fun)) {}

        template<class... Args>
        decltype(auto) operator()(Args &&...args) {
            return fun_(std::ref(*this), std::forward<Args>(args)...);
        }
    };

    template<class Fun>
    decltype(auto) y_combinator(Fun &&fun) {
        return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
    }

} // namespace std

struct UF {
    int n;
    vector<int> par;
    UF(int _n) : n(_n) {
        for (int i = 0; i < n; i++) par.push_back(i);
    }
    int find(int a) {
        if (a != par[a]) par[a] = find(par[a]);
        return par[a];
    }
    bool join(int a, int b) {
        a = find(a);
        b = find(b);
        par[a] = b;
        return a != b;
    }
    void add(int u, int v, int r) {
        join(u, r);
        join(v, r);
        par[u] = r;
        par[v] = r;
    }
};

void solve() {
    int N, M;
    cin >> N >> M;
    vector<vector<pair<int, i64>>> adj(N);
    vector<tuple<i64, int, int>> edges;

    for (int i = 0; i < M; i++) {
        int u, v;
        i64 w;
        cin >> u >> v >> w;
        u--, v--;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        edges.push_back({w, u, v});
    }

    const i64 INF = 1.1e9;

    vector<i64> min_wgt(N, INF);
    UF uf(2 * N - 1);
    vector<vector<int>> ch(2 * N - 1);
    for (auto [w, u, v]: edges) {
        if (uf.find(u) != uf.find(v)) {
            int r = (int) min_wgt.size();
            min_wgt.push_back(INF);
            u = uf.find(u);
            v = uf.find(v);
            uf.add(u, v, r);
            ch[r] = {u, v};
        }
        int a = uf.find(u);
        min_wgt[a] = min(min_wgt[a], w);
    }
    int root = uf.find(0);
    y_combinator([&](auto self, int v, i64 val) -> void {
        min_wgt[v] = min(min_wgt[v], val);
        for (auto w: ch[v]) {
            self(w, min_wgt[v]);
        }
    })(root, INF);

    i64 ans = 0;
    for (auto [w, u, v]: edges) ans += w;
    int td = y_combinator([&](auto self, int v) -> int {
        if (v < N) {
            return adj[v].size() % 2 == 1;
        }
        int cur = 0;
        for (auto w: ch[v]) {
            cur += self(w);
            while (cur >= 2) {
                cur -= 2;
                ans += min_wgt[v];
            }
        }
        return cur;
    })(root);
    assert(td == 0);
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

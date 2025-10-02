#include <bits/stdc++.h>
using namespace std;

static constexpr int MAXN = 5e4 + 10;
using ll = long long;
using pii = pair<int, int>;
// static constexpr int MOD = 1e9 + 7;
static constexpr int MOD = 998244353;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int> > g(n);
    vector<int> indegree(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g[u].push_back(v);
        indegree[v]++;
    }
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }
    vector<ll> f(n, 1);
    vector<bitset<MAXN> > h(n);
    for (int i = 0; i < n; i++) {
        h[i].set(i);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v: g[u]) {
            (f[v] += f[u]) % MOD;
            h[v] |= h[u];
            if (--indegree[v] == 0) {
                q.push(v);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        cout << ((f[i] - (ll) h[i].count() + MOD) % MOD) << " ";
    }
    cout << '\n';
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}

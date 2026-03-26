#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
const int MOD = 998244353;

i64 qpow(i64 a, i64 b) {
    i64 res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

i64 inv(i64 a) { return qpow(a, MOD - 2); }

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> g(n + 1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<int> vec;
    auto dfs = [&](this auto &&dfs, int u, int p) -> int {
        int s = 1;
        for (int v: g[u]) {
            if (v == p) continue;
            s += dfs(v, u);
        }
        if ((s & 1) == 0) {
            vec.push_back(s);
            return 0;
        }
        return s;
    };

    int tot = dfs(1, 0);
    if (tot == 0) {
        tot = vec.back();
        vec.pop_back();
    }
    if (vec.size() == 0) {
        cout << 1 << '\n';
        return;
    }
    i64 ans = 1;
    for (int i = 1; i < vec.size(); i++) {
        ans = ans * i % MOD;
    }
    i64 sum = 0;
    for (int i = 0; i < vec.size(); i++) {
        ans = ans * vec[i] % MOD * vec[i] % MOD;
        sum = (sum + inv(vec[i])) % MOD;
    }
    cout << ans * sum % MOD * tot % MOD << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

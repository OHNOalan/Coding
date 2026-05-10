#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

constexpr int B = 21;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> p(n);
    for (auto &x: p) cin >> x;

    vector<i64> next(n, n);
    stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && p[i] < p[st.top()]) {
            next[st.top()] = i;
            st.pop();
        }
        st.push(i);
    }

    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n; i++) g[next[i]].push_back(i);
    vector<array<int, B>> up(n + 1), pa(n + 1);
    vector<i64> sum1(n + 1), sum2(n + 1);
    for (int i = 0; i <= n; i++) up[i].fill(n), pa[i].fill(n);
    [&](this auto &&dfs, int u) -> void {
        for (int v: g[u]) {
            int d = u - v;
            for (int i = 0; i < B; i++) {
                up[v][i] = (d >= 1 << i) ? v : up[u][i];
                if (i == 0)
                    pa[v][i] = u;
                else
                    pa[v][i] = pa[pa[v][i - 1]][i - 1];
            }
            sum1[v] = sum1[u] + d;
            sum2[v] = sum2[u] + 1LL * d * d;
            dfs(v);
        }
    }(n);

    while (q--) {
        i64 l, r;
        cin >> l >> r;
        l--;
        i64 u = min(r, next[l]);
        i64 t = u - l;
        i64 x = (t - 1) / 2;
        i64 ans = x * (t - 1 - x);
        while (u < r) {
            int i = bit_width((unsigned) t) - 1;
            assert(i < B && i >= 0);
            i64 v = up[u][i];
            if (v > r) {
                v = u;
                for (int j = B - 1; j >= 0; j--)
                    if (pa[v][j] <= r) v = pa[v][j];
            }
            i64 d1 = sum1[u] - sum1[v], d2 = sum2[u] - sum2[v];
            ans += t * d1 + (d1 * d1 - d2) / 2;
            t += d1;
            u = v;
            if (u < r) {
                i64 s = min(r, next[u]) - u;
                i64 x = clamp((s - t) / 2, 0LL, s);
                ans += -x * x + (s - t) * x + s * t;
                t += s;
                u += s;
            }
        }
        cout << ans << "\n";
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

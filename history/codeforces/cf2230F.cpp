#include <bits/stdc++.h>
#include <ostream>

using namespace std;

const int INF = INT_MAX / 2;

void solve() {
    int n;
    cin >> n;
    vector<int> par(n + 1);
    for (int i = 1; i <= n; ++i) cin >> par[i], --par[i];
    vector<vector<int>> allG(n + 1);
    for (int j = 1; j <= n; ++j) {
        allG[par[j]].push_back(j);
    }
    // [0, q]
    // [0, i] vertex
    vector<int> cache(n + 1, -1);
    vector<int> f(n + 1), h(n + 1);
    vector<vector<int>> topK(n + 1);
    for (int x = 0; x <= n; ++x) topK[x].reserve(3);
    auto calc = [&](int I) -> int {
        assert(I >= 0 && I <= n);
        if (cache[I] != -1) return cache[I];
        fill(f.begin(), f.begin() + I + 1, 0);
        for (int x = 0; x <= I; ++x) topK[x].clear();
        // keep mx_i, se_i, th_i
        auto getTop3 = [&](vector<int> &tk, int y) -> void {
            tk.push_back(y);
            for (int p = (int) tk.size() - 1; p > 0; --p) {
                if (f[tk[p]] > f[tk[p - 1]]) {
                    swap(tk[p], tk[p - 1]);
                } else {
                    break;
                }
            }
            if (tk.size() > 3) tk.pop_back();
        };
        for (int x = I; x >= 0; --x) {
            auto &tx = topK[x];
            for (int y: allG[x]) {
                if (y > I) break;
                getTop3(tx, y);
            }
            if (tx.size() > 1) f[x] = f[tx[1]] + 1;
        }
        int ans = 0;
        copy(f.begin(), f.begin() + I + 1, h.begin());


        [&](this auto &&reroot, int x, int fa) -> void {
            ans = max(ans, f[x]);
            for (int y: allG[x]) {
                if (y > I) break;
                // remove current and add parent
                int tmpFx = f[x];
                int tmpFy = f[y];
                auto ttk = topK[x];
                auto it = find(ttk.begin(), ttk.end(), y);
                if (it != ttk.end()) ttk.erase(it);
                f[x] = ttk.size() > 1 ? f[ttk[1]] + 1 : 0;
                getTop3(topK[y], x);
                f[y] = (topK[y].size() > 1) ? f[topK[y][1]] + 1 : 0;
                reroot(y, x);
                f[x] = tmpFx;
                f[y] = tmpFy;
            }
        }(0, -1);
        return cache[I] = ans + 1;
    };

    vector<int> ans(n);
    [&](this auto &&self, int l, int r, int L, int R) -> void {
        if (l >= r) return;
        if (L == R) {
            for (int i = l; i < r; i++) ans[i] = L;
            return;
        }
        int mid = (l + r) / 2;
        ans[mid] = calc(mid + 1);
        self(l, mid, L, ans[mid]);
        self(mid + 1, r, ans[mid], R);
    }(0, n, 0, calc(n));

    for (int x: ans) cout << x << ' ';
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    while (t--) solve();
}

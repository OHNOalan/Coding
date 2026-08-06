#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
const int N = 30;
const int NN = N * (N - 1) / 2 + 1;
vector<bitset<NN>> f(N + 1);

void init() {
    f[0][0] = 1;
    for (int k = 1; k <= N; k++) {
        f[k][0] = 1;
        for (int x = 1; x <= k / 2; x++) {
            int kk = k * (k - 1) / 2;
            for (int i = 0; i <= kk; i++) {
                if (f[x][i]) {
                    f[k] |= f[k - x] << (i + x * (k - x));
                }
            }
        }
    }
    // for (int k = 0; k <= 4; k++) {
    //     cout << k << " ";
    //     for (int i = 0; i <= k * (k - 1) / 2; i++) { cout << f[k][i]; }
    //     cout << endl;
    // }
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> ans(n);
    // [l, r) idx, [lo, hi) val
    auto dfs = [&](auto &&dfs, int lo, int hi, int l, int r, int m) -> void {
        // cout << "A " << lo << " " << hi << " " << l << " " << r << " " << m << endl;
        // assert(lo <= hi);
        // assert(l <= r);
        int k = r - l;
        assert(f[k][m]);
        if (k == 0) {
            return;
        }
        if (k == 1) {
            ans[l] = lo;
            return;
        }
        if (m == 0) {
            for (int i = l, j = lo; i < r; i++, j++) { ans[i] = j; }
            return;
        }
        for (int x = 1; x <= k / 2; x++) {
            int mm = x * (k - x);
            if (mm > m)
                continue;
            int kk = k * (k - 1) / 2;
            for (int i = 0; i <= m && m - mm - i >= 0; i++) {
                if (f[x][i] && f[k - x][m - mm - i]) {
                    dfs(dfs, hi - x, hi, l, l + x, i);
                    dfs(dfs, lo, hi - x, l + x, r, m - mm - i);
                    return;
                }
            }
        }
        assert(false);
    };
    if (f[n][m]) {
        dfs(dfs, 1, n + 1, 0, n, m);
        for (int x: ans) cout << x << " ";
        cout << '\n';
        // int tot = 0;
        // for (int i = 0; i < n; i++)
        //     for (int j = i + 1; j <= n; j++) {
        //         for (int x = i; x < j; x++)
        //             for (int y = x + 1; y < j; y++) {
        //                 if (ans[x] > ans[y]) {
        //                     tot++;
        //                     goto here;
        //                 }
        //             }
        //     here:
        //     }
        // cout << tot << " " << m << endl;
        // assert(tot == m);
    } else
        cout << "0\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();

    int t;
    cin >> t;
    while (t--) solve();
}

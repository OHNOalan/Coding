#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int N = 4e5 + 10;
const int MOD = 1e9 + 7;

i64 dp[N];

void init() {
    dp[0] = 1;
    dp[1] = 2;
    for (int i = 2; i < N; i++) { dp[i] = (2 * dp[i - 1] + (i - 1) * dp[i - 2]) % MOD; }
}

struct DSU {
    int n;
    vector<int> f, sz;

    DSU(int n) : n(n), f(n), sz(n, 1) { iota(f.begin(), f.end(), 0); }

    int find(int x) { return x == f[x] ? x : f[x] = find(f[x]); }

    int getSize(int i) { return sz[find(i)]; }

    void merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y)
            return;
        if (sz[x] > sz[y])
            swap(x, y);
        f[y] = x;
        sz[x] += sz[y];
    }
};

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int &x: a) cin >> x;

    DSU dsu(n + 1);
    vector<int> ind(n + 1);
    i64 ans;
    int m = n;

    for (int i = 0; i <= n; i++) {
        if (a[i] > n) {
            goto finish;
        }
    }

    for (int i = 0; i <= n; i++)
        if (a[i] != -1) {
            dsu.merge(i, a[i]), ind[a[i]]++;
        }

    for (int i = 0; i <= n; i++) {
        if (i > 0 && ind[i] > 1) {
            goto finish;
        }
        if (a[i] > 0 && dsu.getSize(i) > 2) {
            goto finish;
        }

        if (dsu.find(i) != dsu.find(0) && dsu.getSize(i) == 2)
            m--;
        else if (a[i] != -1)
            m--;
    }

    ans = (dsu.getSize(n) == 1) ? (dp[m] - dp[m - 1] + MOD) % MOD : dp[m];
    cout << ans << '\n';
    return;

finish:
    cout << "0\n";
    return;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();

    int t;
    cin >> t;
    while (t--) solve();
}

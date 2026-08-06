#include <bits/stdc++.h>

using namespace std;

#define YES cout << "YES\n"
#define NO cout << "NO\n"

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
    int n, k;
    cin >> n >> k;
    vector<int> a(n), b(n);
    for (int &x: a) cin >> x;
    for (int &x: b) cin >> x;

    DSU dsu(n);
    vector<int> val(n);
    for (int i = k; i < n; i++) {
        if (a[i] == a[i - k]) {
            dsu.merge(i, i - k);
        }
    }

    bool ok = true;
    vector<int> fix(n);
    auto setf = [&](int idx, int val) -> void {
        int r = dsu.find(idx);
        if (fix[r] == 0)
            fix[r] = val;
        else if (fix[r] != val)
            ok = false;
    };
    for (int i = k; i < n && ok; i++) {
        if (a[i] != a[i - k]) {
            setf(i, a[i]);
            setf(i - k, a[i - k]);
        }
    }

    for (int i = 0; i < n && ok; i++) {
        if (b[i] != -1) setf(i, b[i]);
    }

    vector<int> need(n + 1), fixc(n + 1);
    for (int i = 0; i < k && ok; i++) need[a[i]]++;
    for (int i = 0; i < k && ok; i++) fixc[fix[dsu.find(i)]]++;
    for (int i = 1; i <= n && ok; i++) {
        if (fixc[i] > need[i]) ok = false;
    }
    if (ok)
        YES;
    else
        NO;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

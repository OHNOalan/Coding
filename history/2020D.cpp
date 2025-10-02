#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DSU {
    std::vector<int> f, siz;

    DSU() {}
    DSU(int n) {
        init(n);
    }

    void init(int n) {
        f.resize(n);
        std::iota(f.begin(), f.end(), 0);
        siz.assign(n, 1);
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
            return false;
        }
        siz[x] += siz[y];
        f[y] = x;
        return true;
    }

    int size(int x) {
        return siz[find(x)];
    }
};

void solve() {
    int n; int m;
    cin >> n >> m;
    vector<int> a(m), d(m), k(m);
    for(int i = 0; i < m; i++) {
        cin >> a[i] >> d[i] >> k[i];
        a[i]--;
    }

    DSU dsu(n);
    int ans = n;
    for(int x = 1; x <= 10; x++) {
        vector<int> f(n);
        for(int i=0; i<m; i++) {
            if(d[i] == x) {
                f[a[i]]++;
                f[a[i] + k[i] * x]--;
            }
        }
        for(int i=x; i<n; i++) {
            f[i] += f[i-x];
        }
        for(int i=0; i<n; i++) {
            if(f[i]) {
                ans -= dsu.merge(i, i+x);
            }
        }
    }
    cout << ans << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

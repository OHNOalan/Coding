#include <bits/stdc++.h>

using namespace std;

const int M = 1 << 30;

struct DSU {
    vector<int> f, sz, cz;
    int c;
    DSU(int n, int m) : f(n), sz(n, 1), cz(n, 0), c(n) {
        iota(f.begin(), f.end(), 0);
        fill(cz.begin(), cz.begin() + m, 1);
    }
    int find(int x) {
        return x == f[x] ? x : f[x] = find(f[x]);
    }
    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if(x == y) return false;
        if(sz[x] < sz[y]) swap(x, y);
        c--;
        sz[x] += sz[y];
        cz[x] += cz[y];
        f[y] = x;
        return true;
    }
    int mx() {
        return *max_element(cz.begin(), cz.end());
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<array<int, 2>> ps(n);
    unordered_map<int, int> mpX, mpY;
    for(int i=0; i<n; i++) {
        int x, y;
        cin >> x >> y;
        if(mpX.find(x) == mpX.end()) {
            mpX[x] = mpX.size();
        }
        if(mpY.find(y) == mpY.end()) {
            mpY[y] = mpY.size();
        }
        x = mpX[x], y = mpY[y];
        ps[i] = {x, y};
    }
    int X = mpX.size(), Y = mpY.size();
    int M = n + X + Y;
    DSU dsu(M, n);
    for(int i=0; i<n; i++) {
        auto [x, y] = ps[i];
        dsu.merge(i, n + x);
        dsu.merge(i, n + X + y);
    }
    cout << dsu.c - 1 << '\n';
    return 0;
} 

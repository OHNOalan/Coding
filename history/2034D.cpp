#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n; cin >> n;
    vector<int> a(n), b(n);
    int one = -1;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        b[i] = a[i];
        if(a[i] == 1) one = i;
    }
    ranges::sort(b);
    // int ans = 0;
    vector<array<int, 2>> pairs;
    // x : 0, y : 1, z : 2
    stack<int> xy, xz, yx, yz, zx, zy;
    for(int i=0; i<n; i++) {
        pair<int, int> p{a[i], b[i]};
        if(p == make_pair(0, 1)) {
            xy.push(i);
        } else if(p == make_pair(0, 2)) {
            xz.push(i);
        } else if(p == make_pair(1, 0)) {
            yx.push(i);
        } else if(p == make_pair(1, 2)) {
            yz.push(i);
        } else if(p == make_pair(2, 0)) {
            zx.push(i);
        } else if(p == make_pair(2, 1)) {
            zy.push(i);
        }
    }
    while(xy.size() && yz.size() && zx.size()) {
        if(DEBUG) cout << "A\n";
        int x = xy.top(); xy.pop();
        int y = yz.top(); yz.pop();
        int z = zx.top(); zx.pop();
        pairs.push_back({x, z});
        pairs.push_back({y, z});
    }
    while(xz.size() && zy.size() && yx.size()) {
        if(DEBUG) cout << "B\n";
        int x = zy.top(); zy.pop();
        int y = yx.top(); yx.pop();
        int z = xz.top(); xz.pop();
        pairs.push_back({x, z});
        pairs.push_back({y, z});
    }
    while(xy.size() && yx.size()) {
        if(DEBUG) cout << "C\n";
        int x = xy.top(); xy.pop();
        int y = yx.top(); yx.pop();
        pairs.push_back({x, y});
    }
    while(yz.size() && zy.size()) {
        if(DEBUG) cout << "D\n";
        int x = yz.top(); yz.pop();
        int y = zy.top(); zy.pop();
        pairs.push_back({x, y});
    }
    vector<array<int, 2>> spec;
    int final = one;
    while(xz.size() && zx.size()) {
        if(DEBUG) cout << "E\n";
        int x = xz.top(); xz.pop();
        int y = one;
        int z = zx.top(); zx.pop();
        spec.push_back({x, y});
        spec.push_back({x, z});
        one = z;
    }
    if(final != one) spec.push_back({one, final});
    for(int i=spec.size()-1; i>=0; i--) {
        pairs.push_back(spec[i]);
    }
    assert(xy.empty());
    assert(xz.empty());
    assert(yx.empty());
    assert(yz.empty());
    assert(zx.empty());
    assert(zy.empty());
    assert(pairs.size() <= n);
    cout << pairs.size() << '\n';
    for(int i=pairs.size()-1; i>=0; i--) {
        int u = pairs[i][0], v = pairs[i][1];
        cout << u + 1 << " " << v + 1 << '\n';
        swap(a[u], a[v]);
    }
    assert(is_sorted(a.begin(), a.end()));
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

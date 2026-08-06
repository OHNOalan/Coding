#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (auto &x: a) cin >> x;
    for (auto &x: b) cin >> x;
    auto check = [&](int m) -> bool {
        vector<int> c(n);
        for (int i = 0; i < n; i++) {
            c[i] = (a[i] >= m) + (b[i] >= m);
        }
        vector<int> d;
        d.reserve(n);
        for (int x: c)
            if (x != 1) d.push_back(x);
        c.clear();
        int last = -1, cnt = 0;
        for (int x: d) {
            if (x == 2) {
                ++cnt;
            } else {
                assert(x == 0);
                if (last != 0) --cnt;
            }
            last = x;
        }
        return cnt > 0;
    };
    int l = 1, r = 2 * n + 1;
    while (l + 1 < r) {
        int m = (l + r) / 2;
        (check(m) ? l : r) = m;
    }
    cout << l << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

using i64 = long long;


void solve() {

    int n;
    cin >> n;

    int q = 0;

    auto query = [&](int i, int x) -> int {
        q++;
        assert(q <= 2 * n);
        cout << "? " << i << " " << x << endl;
        cin >> x;
        return x;
    };

    int m = 0;
    {
        int x = n;
        while (x) {
            m++;
            x >>= 1;
        }
    }

    int ans = 0;

    set<int> rest;
    for (int i = 1; i < n; i++) rest.insert(i);

    set<int> likely;
    for (int i = 1; i <= n; i++) likely.insert(i);

    for (int b = 0; b < m; b++) {

        set<int> rest0, rest1, likely0, likely1;
        for (int i: rest) {
            int res = query(i, 1 << b);
            (res ? rest1 : rest0).insert(i);
        }
        for (int i: likely) { ((i & (1 << b)) ? likely1 : likely0).insert(i); }
        if (rest0.size() == likely0.size()) {
            ans |= 1 << b;
            rest = rest1;
            likely = likely1;
        } else {
            rest = rest0;
            likely = likely0;
        }
    }
    cout << "! " << ans << endl;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

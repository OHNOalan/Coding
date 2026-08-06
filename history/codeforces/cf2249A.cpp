#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> l(n), r(n), u(n), v(n);
    for (int i = 0; i < n; i++) {
        cin >> l[i] >> r[i] >> u[i] >> v[i];
        --l[i], --r[i], --u[i], --v[i];
    }

    for (int m = n; m > 0; m--) {
        int id = 0;
        for (int i = 0; i < n && id < m; i++) {
            int L = id, R = m - 1 - id;
            if (!(l[i] <= L && L <= r[i]) && !(u[i] <= R && R <= v[i])) id++;
        }
        if (id == m) return void(cout << m << '\n');
    }
    cout << 0 << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

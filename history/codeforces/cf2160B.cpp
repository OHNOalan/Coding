#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<i64> f(n);
    for (i64 &x: f) cin >> x;

    vector<int> a(n);
    iota(a.begin(), a.end(), 1);

    for (int i = 1; i < n; i++) {
        int d = f[i] - f[i - 1];
        int idx = d - 1 == i ? i : i - d;
        assert(idx < n);
        assert(idx >= 0);
        int cur = a[idx];
        a[i] = cur;
    }

    for (int x: a) cout << x << " ";
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

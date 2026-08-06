#include <bits/stdc++.h>

using namespace std;
using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x: a) cin >> x;
    for (int i = 0; i < n; ++i) {
        if (a[i] > i) {
            return void(cout << "NO\n");
        }
    }

    cout << "YES\n";
    vector<vector<int>> op;

    [&](this auto &&self, int k, int s, int t) -> void {
        if (k <= 0) return;
        int u = 6 - s - t;

        if (a[k - 1] == 0) {
            self(k - 1, s, u);
            op.push_back({k, s, t});
            self(k - 1, u, t);
        } else {
            self(k - 1 - a[k - 1], s, u);
            op.push_back({k, s, t});
            self(k - 1 - a[k - 1], u, s);
            self(k - 1, s, t);
        }
    }(n, 1, 3);

    cout << op.size() << "\n";
    for (auto &p: op) cout << p[0] << " " << p[1] << " " << p[2] << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

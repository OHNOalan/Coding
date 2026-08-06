#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (auto &x: a) cin >> x;
    vector<int> ops;
    for (int i = n - 1; i >= 0; --i) {
        if ((a[i] < 0) ^ (ops.size() % 2 == 0)) {
            ops.push_back(i + 1);
        }
    }
    cout << ops.size() << '\n';
    for (auto &x: ops) cout << x << ' ';
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

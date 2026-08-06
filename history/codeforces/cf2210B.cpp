#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n), rp(n);
    for (auto &x: p) cin >> x;
    for (int i = 0; i < n; i++) {
        p[i]--;
        rp[p[i]] = i;
    }
    int ans = 0;
    vector<int> mark(n);
    for (int i = 0; i < n; i++) {
        if (!mark[i]) {
            ans++;
        }
        mark[p[i]] = true;
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

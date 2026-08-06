#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    long long ans = 0, extra = 0, sum = 0;
    for (int i = 0; i < n; i++) {
        ans += a[i];
        a[i] = ((i + 1) << 1) - a[i];
        sum += a[i];
        sum = max(sum, 0ll);
        extra = max(extra, sum);
    }
    cout << ans + extra << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (auto &x: a) cin >> x;
    for (auto &x: b) cin >> x;
    vector<long long> ans(n + 1);
    long long res = 0, sum = 0;
    for (int i = 0; i < n; ++i) {
        ++ans[0];
        ++sum;
        if (a[i] == b[i]) {
            ans[a[i]] += exchange(ans[a[i] - 1], 0);
        } else {
            sum -= exchange(ans[a[i] - 1], 0);
            sum -= exchange(ans[b[i] - 1], 0);
        }
        res += sum;
    }
    cout << res << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

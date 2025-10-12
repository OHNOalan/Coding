#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;
    int t0 = count(s.begin(), s.end(), '0');
    int t1 = count(s.begin(), s.end(), '1');
    int t2 = k - t1 - t0;

    string ans(n, '?');

    auto f = [&](int i) -> char {
        if (i < t0)
            return '-';
        if (i >= n - t1)
            return '-';
        if (k == n)
            return '-';
        if (i < t0 + t2)
            return '?';
        if (i >= n - t1 - t2)
            return '?';
        return '+';
    };

    for (int i = 0; i < n; i++) ans[i] = f(i);
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

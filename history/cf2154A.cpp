#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;
    int ones = 0, ans = 0;
    for (int i = 0; i < n; i++) {
        int x = s[i] - '0';

        if (x == 1 && ones == 0)
            ans++;

        ones += x;
        if (i >= k - 1)
            ones -= s[i - k + 1] - '0';
    }
    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    string s;
    cin >> n >> s;
    int c0 = count(s.begin(), s.end(), '0'), c1 = n - c0;
    int f0 = 0, f1 = 0;
    for (char c: s) {
        c &= 1;
        if (c == (f0 & 1)) f0++;
        if (c == ((f1 & 1) ^ 1)) f1++;
    }
    int ans = INT_MIN;
    {
        int f01 = f0 / 2, f00 = f0 - f01;
        if (abs((c0 - f00) - (c1 - f01)) <= 1) ans = max(ans, f0);
        int f10 = f1 / 2, f11 = f1 - f10;
        if (abs((c0 - f10) - (c1 - f11)) <= 1) ans = max(ans, f1);
    }
    if (f0 > 0) {
        f0--;
        int f01 = f0 / 2, f00 = f0 - f01;
        if (abs((c0 - f00) - (c1 - f01)) <= 1) ans = max(ans, f0);
    }
    if (f1 > 0) {
        f1--;
        int f10 = f1 / 2, f11 = f1 - f10;
        if (abs((c0 - f10) - (c1 - f11)) <= 1) ans = max(ans, f1);
    }
    ans = ans == INT_MIN ? -1 : n - ans;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

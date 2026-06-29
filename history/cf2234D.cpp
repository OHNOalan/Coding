#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    string s, t;
    cin >> s >> t;
    long long z1 = 0, z2 = 0, z3 = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == '0') z1++;
        if (t[i] == '0') z2++;
        if (s[i] == t[i]) z3++;
    }
    z1 = z1 * (n - z1);
    z2 = z2 * (n - z2);
    z3 = z3 * (n - z3);
    long long ans = 0;
    long long num = ((1LL << k) + 1);
    ans += z1 * ((num + 2) / 3);
    ans += z2 * ((num + 1) / 3);
    ans += z3 * (num / 3);
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, q;
    cin >> n >> q;
    string s, t;
    cin >> s >> t;
    vector<int> cs(n + 1), ct(n + 1), same0(n + 1), same1(n + 1);
    for (int i = 0; i < n; i++) {
        cs[i + 1] = cs[i] + (s[i] == '1');
        ct[i + 1] = ct[i] + (t[i] == '1');
        same0[i + 1] = same0[i] + (s[i] == '0' && s[i] == t[i]);
        same1[i + 1] = same1[i] + (s[i] == '1' && s[i] == t[i]);
    }
    while (q--) {
        int l, r;
        cin >> l >> r, --l;
        int ss0 = same0[r] - same0[l];
        int ss1 = same1[r] - same1[l];
        int s1 = cs[r] - cs[l];
        int t1 = ct[r] - ct[l];
        int c10 = s1 - ss1;
        int c01 = t1 - ss1;
        int len = r - l;
        cout << ((2 * max(c10, c01) <= len) ? "YES\n" : "NO\n");
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

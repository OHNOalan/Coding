#include <bits/stdc++.h>

using namespace std;

void solve() {
    string s;
    cin >> s;
    int pre = 0, suf = 0;
    for (char c: s) {
        suf += c & 1;
    }
    int ans = pre + suf;
    for (char c: s) {
        pre += (c & 3) == 2;
        suf -= c & 1;
        ans = max(ans, pre + suf);
    }
    cout << s.size() - ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

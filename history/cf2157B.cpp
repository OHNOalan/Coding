#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, x, y;
    cin >> n >> x >> y;
    string s;
    cin >> s;
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    reverse(s.begin(), s.end());
    for (char c: s) {
        if (c == '4') {
            auto &z = (x > y ? x : y);
            z = max(0, z - 1);
        } else {
            x = max(0, x - 1);
            y = max(0, y - 1);
        }
    }
    if (x == 0 && y == 0)
        cout << "YES\n";
    else
        cout << "NO\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

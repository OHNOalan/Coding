#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    if (a == b && b == c && c == d)
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

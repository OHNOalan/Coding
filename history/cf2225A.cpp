#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

#define YES cout << "YES\n"
#define NO cout << "NO\n"

void solve() {
    i64 x, y;
    cin >> x >> y;
    y % x == 0 && 2 * x == y ? NO : YES;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

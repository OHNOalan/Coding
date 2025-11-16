#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    i64 a, b, n;
    cin >> a >> b >> n;
    // cout << a << b << n << endl;
    if (b * n <= a || b == a) {
        cout << 1 << '\n';
        return;
    }
    cout << 2 << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

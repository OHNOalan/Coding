#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n, m; cin >> n >> m;
    if(n < m || (n + m) & 1) {
        cout << "-1\n";
    } else {
        int x = (n + m) / 2;
        int y = (n - m) / 2;
        if((x ^ y) != m || (x + y) != n) {
            cout << "-1\n";
            return;
        }
        cout << x << " " << y << '\n';
    }
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

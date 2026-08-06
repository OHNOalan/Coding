#include <bits/stdc++.h>
using namespace std;

void solve() {
    int a, b;
    cin >> a >> b;
    cout << max(0, min(a, 2 * a - b)) << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }

    return 0;
}
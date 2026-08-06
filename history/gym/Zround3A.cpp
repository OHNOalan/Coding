#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n; cin >> n;
    if(n % 11 == 0) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

#define MULTICASE true
int main() {
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
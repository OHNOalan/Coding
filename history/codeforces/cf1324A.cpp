#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    bool odds = false, evens = false;
    for (int x : a) {
        if(x % 2) odds = true;
        else evens = true;
    }
    if(!(odds && evens)) cout << "YES\n";
    else cout << "NO\n";
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

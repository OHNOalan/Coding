#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    map<int, int> pos;
    bool ans = false;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        if(pos.count(x)) {
            if(i - pos[x] > 1) {
                ans = true;
            }
        } else {
            pos[x] = i;
        }
    }
    if(ans) cout << "YES\n";
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

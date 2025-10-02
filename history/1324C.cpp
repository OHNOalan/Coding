#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    string s; cin >> s;
    int n = s.size();
    auto check = [&](int d) -> bool {
        int mx = d;
        for(int i=1; i<=mx && i<=n; i++) {
            char c = s[i-1];
            if(c == 'R') {
                mx = max(mx, i + d);
            }
        }
        return mx >= n + 1;
    };
    int l = 0, r = n + 1;
    while(l + 1 < r) {
        int m = (l + r) / 2;
        (check(m) ? r : l) = m;
    }
    cout << r << '\n';
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

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

void solve() {
    int n, m, k; cin >> n >> m >> k;
    string s; cin >> s;
    int ans = 0;
    int cnt = 0;
    for(int i=0; i<n; i++) {\
        if(s[i] == '0') {
            cnt++;
            if(cnt == m) {
                ans++;
                cnt = 0;
                i += k-1;
            }
        } else {
            cnt = 0;
        }
    }
    cout << ans << '\n';
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

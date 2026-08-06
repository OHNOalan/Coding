#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n; cin >> n;
    map<int, int> mp;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        mp[x]++;
    }
    int ans = 0;
    int ones = 0;
    for(auto [_, cnt] : mp) {
        if(cnt == 1)
            ones++;
        else
            ans++;
    }
    ans += (ones + 1) / 2 * 2;
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

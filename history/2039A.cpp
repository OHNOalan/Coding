#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 1e7 + 10;

using ll = long long;

void solve() {
    int n; cin >> n;
    vector<int> ans{2};
    ans.reserve(n);
    for(int mod=2; mod<=n; mod++) {
        int x = ans.back();
        x -= x % mod;
        x += mod - 1;
        ans.push_back(x);
    }
    for(int x : ans) cout << x << " "; cout << '\n';
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

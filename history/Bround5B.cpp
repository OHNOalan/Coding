#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    vector<int> ans(n-1);
    iota(ans.begin(), ans.end(), 1);
    int k = -1, m = n;
    while(m) {
        m /= 2;
        k++;
    }
    cout << n << " ";
    if(n == (1 << (1 + k)) - 1) {
        for(int x : ans) cout << x << " "; cout << '\n';
    } else {
        int y = (1 << k) - 1;
        cout << y << " ";
        for(int x : ans) if(x != y) cout << x << " "; cout << '\n';
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

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    vector<array<int, 2>> xy(n);
    vector<int> id(n);
    for (int i = 0; i < n; i++) cin >> xy[i][0];
    for (int i = 0; i < n; i++) cin >> xy[i][1];
    iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](int i, int j) {
        return xy[i][1] < xy[j][1] || xy[i][1] == xy[j][1] && xy[i][0] < xy[j][0];
    });
    int mx = INT_MIN;
    vector<int> ans;
    for(int i=n-1; i>=0; i--) {
        if(xy[id[i]][0] >= mx) {
            ans.push_back(id[i]);
        }
        mx = max(mx, xy[id[i]][0]);
    }
    sort(ans.begin(), ans.end());
    cout << ans.size() << '\n';
    for(int x : ans) cout << x + 1 << " "; cout << '\n';
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

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void init() {}

void solve() {
    int n; cin >> n;
    vector<int> p(n+1);
    for(int i=1; i<=n; i++) {
        cin >> p[i];
    }
    vector<int> vis(n+1);
    vector<array<int,2>> intervals;
    for(int i=1; i<=n; i++) {
        if(!vis[i]) {
            int cur = i, mn = i, mx = i;
            while(!vis[cur]) {
                mn = min(mn, cur);
                mx = max(mx, cur);
                vis[cur] = true;
                cur = p[cur];
            }
            if(mn < mx) intervals.push_back({mn, mx});
        }
    }
    if(intervals.empty()) {
        cout << "0\n";
        return;
    }
    sort(intervals.begin(), intervals.end());
    vector<array<int,2>> disjoint;
    disjoint.push_back(intervals[0]);
    for(int i=1; i<intervals.size(); i++) {
        array<int,2>& last = disjoint.back();
        if(intervals[i][0] <= last[1]) {
            last[1] = max(last[1], intervals[i][1]);
        } else {
            disjoint.push_back(intervals[i]);
        }
    }
    int ans = 0;
    for(auto [st, ed] : disjoint) {
        ans += ed - st + 1;
    }
    cout << ans << '\n';
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
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    string s; cin >> s;
    string t; cin >> t;
    int n = s.size();
    int m = t.size();
    vector<vector<int>> pre(m+1, vector<int> (n+1, 0));
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            if(t[i] == s[j]) {
                pre[i+1][j+1] = pre[i][j] + 1;
            } else {
                pre[i+1][j+1] = max(pre[i+1][j], pre[i][j+1]);
            }
        }
    }
    vector<vector<int>> suf(m+1, vector<int> (n+1, 0));
    for(int i=m-1; i>=0; i--) {
        for(int j=n-1; j>=0; j--) {
            if(t[i] == s[j]) {
                suf[i][j] = suf[i+1][j+1] + 1;
            } else {
                suf[i][j] = max(suf[i+1][j], suf[i][j+1]);
            }
        }
    }
    for(int i=0; i<m; i++) {
        int ans = 0;
        for(int j=0; j<=n; j++) {
            ans = max(ans, min(n, pre[i][j] + suf[i+1][j]));
        }
        cout << ans << " \n"[i == m-1];
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

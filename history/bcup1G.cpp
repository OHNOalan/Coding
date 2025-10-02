#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n; cin >> n;
    string s; cin >> s;
    if(s[0] == '1' || s[n-1] == '0') {
        cout << "NO\n";
        return;
    }
    vector<int> ans;
    for(int i=0; i<n; i++) {
        int st = i;
        while(s[i+1] == '0') i++;
        int mid = i++;
        while(i<n-1 && s[i+1] == '1') i++;
        for(int j=st; j<mid; j++) {
            ans.push_back(j);
        }
        for(int j=i-1; j>=mid; j--) {
            ans.push_back(j);
        }
    }
    cout << "YES\n";
    cout << ans.size() << '\n';
    for(int x : ans) cout << x + 1 << " ";
    cout << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
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

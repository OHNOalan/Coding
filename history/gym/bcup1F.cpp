#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n; cin >> n;
    string s; cin >> s;
    if(s[0] == '1') {
        cout << "NO\n";
        return;
    }
    vector<int> ans;
    for(int i=n-1; i; i--) {
        if(s[i] == '1') {
            ans.push_back(i);
        }
    }
    cout << "YES\n";
    cout << ans.size() << '\n';
    for(int x : ans) cout << x << " ";
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

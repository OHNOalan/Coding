#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 1e7 + 10;

using ll = long long;

void solve() {
    string s; cin >> s;
    int n = s.size();
    for(int i=0; i<n-1; i++) {
        if(s[i] == s[i+1]) {
            cout << s.substr(i, 2) << '\n';
            return;
        }
    }
    for(int i=0; i<n-2; i++) {
        if(s[i] != s[i+1] && s[i+1] != s[i+2] && s[i] != s[i+2]) {
            cout << s.substr(i, 3) << '\n';
            return;
        }
    }
    cout << "-1\n";
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

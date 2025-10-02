#include <bits/stdc++.h>
using namespace std;

void solve() {
    int k;
    cin >> k;
    string s;
    if(k & 1) {
        s.push_back('+');
        k--;
    }
    while(k) {
        s.push_back('-');
        s.push_back('+');
        k -= 2;
    }
    cout << s << '\n';
}

#define MULTICASE true
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
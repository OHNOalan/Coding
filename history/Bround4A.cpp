#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2010;
using ll = long long;

void solve() {
    string s; cin >> s;
    vector<int> cnt(26);
    for(char b : s) {
        cnt[b - 'a']++;
    }
    int odds = 0;
    for(int x : cnt)
        odds += x & 1;
    cout << max(0, odds - 1) + 1 << '\n';
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
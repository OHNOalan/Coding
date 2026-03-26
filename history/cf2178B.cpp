#include <bits/stdc++.h>

using namespace std;

#define YES cout << "YES\n"
#define NO cout << "NO\n"

void solveS(string s, int inc) {
    stringstream ss(s);
    string t;
    int ans = 0;
    while (getline(ss, t, 's')) {
        ans += t.size() / 2;
    }
    cout << ans + inc << '\n';
}

void solve() {
    string s;
    cin >> s;
    if (s.front() == 'u' && s.back() == 'u')
        solveS('s' + s.substr(1, s.size() - 2) + 's', 2);
    else if (s.front() == 'u')
        solveS('s' + s.substr(1), 1);
    else if (s.back() == 'u')
        solveS(s.substr(0, s.size() - 1) + 's', 1);
    else
        solveS(s, 0);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

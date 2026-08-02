#include <bits/stdc++.h>

using namespace std;

void solve() {
    string s;
    cin >> s;
    s.erase(s.find('0'), 1);
    s.erase(s.find('1'), 1);
    cout << s << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

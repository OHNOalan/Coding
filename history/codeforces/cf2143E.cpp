#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    string s;
    cin >> s;
    if (n % 2) {
        cout << "-1\n";
        return;
    }

    int open = 0, close = 0;
    for (int i = 0; i < s.size(); i++) {
        char &c = s[i];
        if (i % 2) c ^= 1;
        (c & 1 ? close : open)++;
    }

    if (open % 2 == 1 || open == 0) {
        cout << "-1\n";
        return;
    }
    string t = '(' + string(close, ')') + string(open - 1, '(');
    for (int i = 0; i < t.size(); i++)
        if (i % 2) t[i] ^= 1;
    cout << t << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

#define YES cout << "YES\n"
#define NO cout << "NO\n"

void solve() {
    string s;
    cin >> s;

    int n = s.size();

    auto f = [&] {
        int l = 0, r = n - 1;
        for (char c = 'a'; l < n && s[l] == c; ++l, c = c == 'b' ? 'a' : 'b');
        for (char c = (n % 2 == 0) ? 'b' : 'a'; r >= 0 && s[r] == c; --r, c = c == 'b' ? 'a' : 'b');
        if (l > r) return true;
        char c = s[r];
        for (int i = r; i >= l; --i) {
            if (s[i] != c) return false;
            c = c == 'b' ? 'a' : 'b';
        }
        return true;
    };

    if (f()) {
        YES;
        return;
    }
    for (char &c: s) {
        c = c == 'b' ? 'a' : 'b';
    }
    if (f()) {
        YES;
        return;
    }
    NO;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

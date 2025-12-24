#include <bits/stdc++.h>

using namespace std;

void solve() {
    string s;
    cin >> s;
    int n = s.size();
    int l = 0, r = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == '<')
            l++;
        else
            break;
    }
    for (int i = n - 1; i >= 0; i--) {
        if (s[i] == '>')
            r++;
        else
            break;
    }
    if (l + r < n - 1)
        cout << -1 << "\n";
    else
        cout << n - l - r + max(l, r) << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

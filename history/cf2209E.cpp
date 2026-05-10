#include <bits/stdc++.h>

using namespace std;


void get(string_view s) {
    int n = s.size();
    vector<int> p(n), b(n);
    for (int i = 1; i < n; i++) {
        int j = p[i - 1];
        while (j > 0 && s[i] != s[j]) j = p[j - 1];
        if (s[i] == s[j]) j++;
        p[i] = j;
        if (j == 0)
            b[i] = 0;
        else if (p[j - 1] == 0)
            b[i] = j;
        else
            b[i] = b[j - 1];
    }
    long long ans = 0;
    for (int i = 0; i < n; i++) {
        p[i] = 0;
        p[i] = p[i - b[i]] + 1;
        ans += p[i];
    }
    cout << ans << '\n';
}

void solve() {
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;
    int l, r;
    while (q--) {
        cin >> l >> r;
        get(string_view(s.begin() + l - 1, s.begin() + r));
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

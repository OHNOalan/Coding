#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    string s, t;
    cin >> s >> t;

    vector<int> pre(n);
    int j = n - 1;
    for (int i = n - 1; i >= 0; i--) {
        while (j > i || j >= 0 && s[j] != t[i]) j--;
        if (j == -1) {
            cout << "-1\n";
            return;
        }
        pre[i] = j;
    }
    int mx = 0;
    for (int i = 0; i < n; i++) mx = max(mx, i - pre[i]);
    if (mx > k) {
        cout << "-1\n";
        return;
    }

    cout << mx << '\n';
    for (int r = 0; r < mx; r++) {
        for (int i = n - 1; i > 0; i--) {
            if (pre[i] != i) {
                s[i] = s[i - 1];
                pre[i]++;
            }
        }
        cout << s << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

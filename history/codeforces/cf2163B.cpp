#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int &x: p) cin >> x;
    string s;
    cin >> s;
    for (int i = 0; i < n; i++) {
        if (s[i] == '1' && (i == 0 || i == n - 1 || p[i] == 1 || p[i] == n)) {
            cout << "-1\n";
            return;
        }
    }
    int a = find(p.begin(), p.end(), 1) - p.begin() + 1;
    int b = find(p.begin(), p.end(), n) - p.begin() + 1;

    if (a > b) swap(a, b);

    cout << 5 << '\n';
    cout << 1 << " " << a << '\n';
    cout << 1 << " " << b << '\n';
    cout << a << " " << b << '\n';
    cout << a << " " << n << '\n';
    cout << b << " " << n << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

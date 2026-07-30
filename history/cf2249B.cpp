#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n - 1);
    vector<int> c(n);
    vector<int> l(n, -1), r(n, -1);
    int mx = -1;
    for (int i = 0, x; i < n - 1; i++) {
        cin >> x;
        a[i] = --x;
        c[x]++;
        if (l[x] == -1) l[x] = i;
        r[x] = i;
        mx = max(mx, x);
    }
    if (mx != n - 2) return void(cout << 0 << '\n');
    for (int i = 0, d = 1; i < n - 2; i++) {
        if (a[i] * d > a[i + 1] * d) {
            if (d == -1)
                return void(cout << 0 << '\n');
            else
                d = -1;
        }
    }
    for (int i = 0; i < n; i++) {
        if (c[i] > 0 && r[i] - l[i] + 1 != c[i]) return void(cout << 0 << '\n');
    }

    long long ans = 2;
    long long slots = c[n - 2] - 1;

    for (int x = n - 3; x >= 0; x--) {
        if (c[x] > 0)
            slots += c[x] - 1;
        else {
            ans = ans * slots % 998244353;
            slots--;
        }
    }

    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

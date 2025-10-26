#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    vector<int> f(n + 1), suf(n + 1);
    for (int x: a) f[x]++;
    suf[n] = f[n];
    for (int i = n - 1; i >= 0; i--) { suf[i] = suf[i + 1] + f[i]; }
    for (int g = n; g >= 2; g--) {
        int tot = f[g];
        if (2 * g <= n)
            tot += f[2 * g];
        if (3 * g <= n)
            tot += f[3 * g];
        if (4 * g <= n)
            tot += suf[4 * g];
        if (tot >= n - k) {
            cout << g << "\n";
            return;
        }
    }
    cout << 1 << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

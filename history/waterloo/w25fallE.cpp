#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

bool solve() {
    int n, m;
    cin >> n >> m;
    if (n == 0 && m == 0)
        return false;
    vector<int> a(n), b(m);
    for (int &x: a) cin >> x;
    for (int &x: b) cin >> x;
    if (n > m) {
        cout << "Impossible\n";
        return true;
    }
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    i64 ans = 0;
    for (int i = 0, j = 0; i < n; i++) {
        while (j < m && a[i] > b[j]) j++;
        if (j == m) {
            cout << "Impossible\n";
            return true;
        }
        assert(a[i] <= b[j]);
        ans += b[j];
        j++;
    }
    cout << ans << '\n';
    return true;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    while (solve());
}

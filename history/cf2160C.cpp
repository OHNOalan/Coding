#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a;
    while (n && (n & 1) == 0) n >>= 1;
    while (n) {
        a.push_back(n & 1);
        n >>= 1;
    }
    n = a.size();
    for (int i = 0; i < n / 2; i++) {
        if (a[i] != a[n - 1 - i]) {
            cout << "NO\n";
            return;
        }
    }
    if (n % 2 == 0 || a[n / 2] == 0) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

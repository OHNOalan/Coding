#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    for (int i = 0, j = n - 1, k = 1; i <= j;) {
        if (a[i] == k) {
            i++, k++;
        } else if (a[j] == k) {
            j--, k++;
        } else {
            cout << "NO\n";
            return;
        }
    }
    cout << "YES\n";
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

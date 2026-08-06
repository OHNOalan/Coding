#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    vector<int> b(k);
    for (int &x: a) cin >> x;
    for (int &x: b) cin >> x;
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    int i = n - 1, j = 0;
    i64 cost = 0;
    while (i >= 0 && j < k) {
        if (i + 1 >= b[j]) {
            for (int x = i; x > i - b[j] + 1; x--) {
                cost += a[x];
            }
            i -= b[j];
            j++;
        } else {
            break;
        }
    }
    while (i >= 0) {
        cost += a[i];
        i--;
    }
    cout << cost << '\n';
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

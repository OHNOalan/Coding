#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    vector<int> b(n);
    for (int i = 0; i < n; ++i)
        cin >> a[i] >> b[i];
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);

    sort(idx.begin(), idx.end(), [&](int i, int j) {
        int mx_i = max(a[i], b[i]);
        int mn_i = min(a[i], b[i]);
        int mx_j = max(a[j], b[j]);
        int mn_j = min(a[j], b[j]);
        return mx_i < mx_j || mx_i == mx_j && mn_i < mn_j;
    });

    for (int i : idx) {
        cout << a[i] << " " << b[i] << " ";
    }
    cout << endl;
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}
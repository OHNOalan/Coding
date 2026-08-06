#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, x;
    cin >> n >> x;
    if ((n & (n - 1)) != 0) return void(cout << -1 << "\n");

    if (x == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << (i ^ j) << " ";
            }
            cout << "\n";
        }
        return;
    }

    if (n == 2) return void(cout << -1 << "\n");

    vector<int> c(n);
    vector<bool> used(n);
    int j0 = 0, j1 = 1;
    for (int i = 0; i < n; i++) {
        if (used[i]) continue;
        int y = i;
        int z = i ^ x;
        used[y] = used[z] = true;
        int &j = j1 < n ? j1 : j0;
        c[j] = y;
        j += 2;
        c[j] = z;
        j += 2;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << (c[i] ^ c[j] ^ ((i & 1 * j & 1) * x)) << " ";
        }
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, k;
    cin >> n >> k;
    if (k == n - 1) return void(cout << "-1\n");
    k = n - k;
    int c0 = (n + 1) / 2, c1 = n / 2;
    for (int i = 1; i <= k; ++i) {
        if (i & 1) {
            if (i + 2 > k) {
                while (c0--) cout << 0;
            } else {
                --c0;
                cout << 0;
            }
        } else {
            if (i + 2 > k) {
                while (c1--) cout << 1;
            } else {
                --c1;
                cout << 1;
            }
        }
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

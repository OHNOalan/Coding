#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int A;
        cin >> A;
        cout << min(n, 2) << ' ';
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

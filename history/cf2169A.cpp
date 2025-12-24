#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, a;
    cin >> n >> a;
    int x = 0, y = 0;
    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;
        if (c < a)
            x++;
        else if (c > a)
            y++;
    }
    cout << (x > y ? a - 1 : a + 1) << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

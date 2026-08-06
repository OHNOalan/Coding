#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> c(n);
    for (auto &x: c) cin >> x;
    long long ones = 0, spaces = 0, seg = 0, ans = 0;
    for (int x: c) {
        if (x > 1) {
            ans += x;
            spaces += x / 2 - 1;
            ++seg;
        } else {
            ++ones;
        }
    }
    ans += min(ones, spaces + (seg == 1));
    cout << ((ans >= 3) ? ans : 0) << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

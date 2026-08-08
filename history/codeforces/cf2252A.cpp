#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    map<int, int> cnt;
    int sum = 0, mx = 0, val = 0;
    for (int i = 0, x; i < n; ++i) {
        cin >> x;
        sum += x;
        cnt[x]++;
        if (cnt[x] > mx) {
            mx = cnt[x];
            val = x;
        }
    }
    sum -= max(0, 2 * mx - n - 2) * val;
    cout << sum << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

constexpr long long N = 1e12;

void solve() {
    long long x, y, k;
    cin >> x >> y >> k;
    if (y == 1) {
        cout << "-1\n";
        return;
    }
    for (long long i = 0; i < x;) {
        long long cur = (k - 1) / (y - 1);
        if (cur == 0) break;
        long long fk = (cur + 1) * (y - 1) + 1;
        long long cnt = (fk - k + cur - 1) / cur;
        cnt = min(x - i, cnt);
        k += cnt * cur;
        if (k > N) {
            cout << "-1\n";
            return;
        }
        i += cnt;
    }
    cout << k << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

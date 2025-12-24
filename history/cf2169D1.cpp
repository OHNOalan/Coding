#include <bits/stdc++.h>

using namespace std;

constexpr long long N = 1e12;

void solve() {
    long long x, y, k;
    cin >> x >> y >> k;
    auto check = [&](long long cnt) -> long long {
        for (int i = 0; i < x && cnt > 0; i++) {
            cnt -= cnt / y;
        }
        return cnt;
    };
    if (check(N) < k) {
        cout << "-1\n";
        return;
    }
    long long l = 0, r = N + 1;
    while (l + 1 < r) {
        long long m = (l + r) / 2;
        (check(m) >= k ? r : l) = m;
    }
    cout << r << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

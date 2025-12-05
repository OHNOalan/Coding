#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    vector<pair<int, int>> ans;
    auto add = [&](int x, int d) {
        if (x <= 250000 and d <= 250000) {
            ans.push_back({x, d});
        }
    };
    for (int r = 1; r < 64; r++) {
        for (int i = 64 * 64 * 64 + r; i >= 1; i -= 64) {
            add(i, 1);
        }
    }
    for (int r = 64; r < 64 * 64; r += 64) {
        for (int i = 64 * 64 * 64 + r; i >= 1; i -= 64 * 64) {
            add(i, 64);
        }
    }
    for (int r = 64 * 64; r < 64 * 64 * 64; r += 64 * 64) {
        for (int i = 64 * 64 * 64 + r; i >= 1; i -= 64 * 64 * 64) {
            add(i, 64 * 64);
        }
    }
    cout << ans.size() << '\n';
    for (auto [x, d]: ans) {
        cout << x << " " << d << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    solve();
}

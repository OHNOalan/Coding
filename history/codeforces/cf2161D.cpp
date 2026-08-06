#include <bits/stdc++.h>

using namespace std;

using i64 = long long;


void solve() {
    int n;
    cin >> n;
    vector<vector<int>> vec(n);
    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        x--;
        vec[x].push_back(i);
    }
    vector<int> bit(n + 2);
    auto add = [&](int i, int val) -> void {
        for (i++; i <= n + 1; i += i & -i) bit[i] = max(bit[i], val);
    };
    auto query = [&](int i) -> int {
        int ans = 0;
        for (i++; i > 0; i -= i & -i) ans = max(ans, bit[i]);
        return ans;
    };

    for (int x = n - 1; x >= 0; x--) {
        int res = query(n);
        for (auto i: vec[x]) add(i, query(i - 1) + 1);
        add(0, res);
    }
    cout << (n - query(n)) << '\n';
}


void solve2() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) {
        cin >> x;
        x--;
    }
    vector<vector<int>> at(n);
    for (int i = 0; i < n; i++) at[a[i]].push_back(i);

    int ans = 0;
    for (int v = 0; v < n - 1; v++) {
        int mn = min(at[v].size(), at[v + 1].size());
        int left = 0, right = mn + 1;
        while (left + 1 < right) {
            int mid = (left + right) / 2;
            bool ok = true;
            for (int i = 0; i < mid; i++) {
                if (at[v][i] > at[v + 1][at[v + 1].size() - mid + i]) {
                    ok = false;
                    break;
                }
            }
            (ok ? left : right) = mid;
        }
        ans += left;
        at[v + 1].resize(at[v + 1].size() - left);
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

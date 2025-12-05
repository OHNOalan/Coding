#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(2 * n);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        x--;
        a[x]++;
    }

    int ans = 0;
    vector<pair<int, int>> ones;
    for (int i = 2 * n - 1; i >= 0; i--) {
        if (a[i] == 0) continue;
        pair<int, int> cur{i, i + a[i]};
        int need = max(a[i] - k, 0), tot = 0;
        while (!ones.empty() && cur.second >= ones.back().first) {
            auto [l, r] = ones.back();
            ones.pop_back();
            if (need) {
                tot += min(need, l - cur.first);
                need -= l - cur.first;
                if (need >= 0) {
                    tot += r - l;
                }
                need = max(need, 0);
            }
            cur.second += r - l;
            cur.first = r;
            assert(need >= 0);
        }
        ans = max(ans, tot + need);
        cur.first = i;
        ones.push_back(cur);
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

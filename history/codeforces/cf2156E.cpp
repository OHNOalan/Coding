#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    vector<int> sa = a;
    sa.erase(unique(sa.begin(), sa.end()), sa.end());
    sort(sa.begin(), sa.end());
    int m = sa.size();
    auto gx = [&](int x) -> int { return lower_bound(sa.begin(), sa.end(), x) - sa.begin(); };

    int left = -1e9, right = 1e9;
    while (left + 1 < right) {
        int goal = (left + right) / 2;
        vector<int> bit(m + 1);
        auto lowbit = [&](int i) -> int { return i & -i; };
        auto update = [&](int i, int val) -> void {
            for (i++; i <= m; i += lowbit(i)) bit[i] += val;
        };
        auto query = [&](int i) -> int {
            int ans = 0;
            for (i++; i; i -= lowbit(i)) ans += bit[i];
            return ans;
        };

        vector<int> f(n);

        auto without_j = [&](int j) -> void {
            bit.assign(m + 1, 0);
            for (int i = 0; i < n; i++)
                if (i != j) {
                    f[i] += query(gx(a[i] - goal + 1) - 1);
                    update(gx(a[i]), 1);
                }
            bit.assign(m + 1, 0);
            for (int i = n - 1; i >= 0; i--)
                if (i != j) {
                    f[i] += query(m - 1) - query(gx(a[i] + goal) - 1);
                    update(gx(a[i]), 1);
                }
        };
        without_j(-1);

        vector<int> js;
        for (int i = 0; i < n; i++) {
            if (f[i] >= 2) {
                js.push_back(i);
                if (f[i] > 2) {
                    js = {i};
                    break;
                }
            }
        }

        bool achieve = false;
        if (!js.empty()) {
            if (js.size() > 3) {
                achieve = true;
            } else {
                bool allwork = true;
                for (int j: js) {
                    f.assign(n, 0);
                    without_j(j);
                    bool work = false;
                    for (int i = 0; i < n; i++) {
                        if (f[i] >= 2) {
                            work = true;
                            break;
                        }
                    }
                    if (!work) {
                        allwork = false;
                        break;
                    }
                }
                if (allwork)
                    achieve = true;
            }
        }

        (achieve ? left : right) = goal;
    }
    cout << left << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

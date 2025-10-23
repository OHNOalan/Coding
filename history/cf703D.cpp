#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    vector<int> pre(n + 1);
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] ^ a[i];

    int m;
    cin >> m;
    vector<array<int, 2>> qs(m);
    for (auto &q: qs) {
        cin >> q[0] >> q[1];
        q[0]--;
    }
    vector<int> ans(m);
    vector<int> idx(m);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](const int i, const int j) -> bool { return qs[i][1] < qs[j][1]; });

    vector<int> bit(n + 1);

    auto lowbit = [](int i) -> int { return i & -i; };

    auto update = [&](int i, int val) -> void {
        for (i++; i <= n; i += lowbit(i)) { bit[i] ^= val; }
    };

    auto query = [&](int i) -> int {
        int ans = 0;
        for (; i > 0; i -= lowbit(i)) { ans ^= bit[i]; }
        return ans;
    };

    int r = 0;
    map<int, int> lst;
    for (int i = 0; i < m; i++) {
        int ql = qs[idx[i]][0], qr = qs[idx[i]][1];
        while (r < qr) {
            int x = a[r];
            if (lst.find(x) != lst.end()) {
                update(lst[x], x);
            }
            update(r, x);
            lst[x] = r;
            r++;
        }
        ans[idx[i]] = pre[qr] ^ pre[ql] ^ query(qr) ^ query(ql);
    }

    for (int x: ans) cout << x << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    solve();
}

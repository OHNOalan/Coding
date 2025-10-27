#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
const int INF = 1e9;

struct Segtree {
    // zero-index
    // [l, r)
#define LF v * 2 + 1
#define RT v * 2 + 2
    int n;

    vector<int> arr;

    Segtree(int _n) : n(_n), arr(4 * n) {}

    Segtree(vector<int> &a) : n(a.size()), arr(4 * n) { build(a); }

    void build(vector<int> &a, int l, int r, int v) {
        if (l + 1 == r) {
            arr[v] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(a, l, m, LF);
        build(a, m, r, RT);
        arr[v] = min(arr[LF], arr[RT]);
    }

    void build(vector<int> &a) { build(a, 0, n, 0); }

    void update(int v, int l, int r, int i, int val) {
        if (l + 1 == r) {
            arr[v] = val;
            return;
        }
        int m = (l + r) / 2;
        if (i < m)
            update(LF, l, m, i, val);
        else
            update(RT, m, r, i, val);
        arr[v] = min(arr[LF], arr[RT]);
    }

    void update(int i, int val) { update(0, 0, n, i, val); }

    int query(int v, int l, int r, int L, int R) {
        if (l + 1 == r) {
            return arr[v];
        }
        if (L <= l && r <= R) {
            return arr[v];
        }
        int m = (l + r) / 2;
        if (R <= m)
            return query(LF, l, m, L, R);
        else if (m <= L)
            return query(RT, m, r, L, R);
        return min(query(LF, l, m, L, R), query(RT, m, r, L, R));
    }

    int query(int L, int R) { return query(0, 0, n, L, R); }
};

struct BIT {
    int n;
    vector<int> bit;
    BIT(int _n) : n(_n), bit(n + 1) {}

    int lowbit(int i) { return i & -i; }
    void update(int i, int val) {
        for (i++; i <= n; i += lowbit(i)) bit[i] += val;
    }
    int query(int i) {
        int ans = 0;
        for (i++; i; i -= lowbit(i)) ans += bit[i];
        return ans;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<int> p(n), ans(n, -1), rp(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
        p[i]--;
        rp[p[i]] = i;
    }

    BIT bit(n);
    Segtree seg(rp);
    for (int i = 0; i < n; i++) bit.update(i, 1);
    int cur = 1;
    for (int i = 0; i < n; i++) {
        if (ans[i] != -1)
            continue;

        int idx = bit.query(p[i]) - 1;
        if (idx % 2 == 0) {
            bit.update(p[i], -1);
            seg.update(p[i], INF);
            ans[i] = cur++;
            continue;
        }
        int j = i;

        while (true) {
            j = seg.query(0, p[j]);
            int idx = bit.query(p[j]) - 1;
            if (idx % 2 == 0)
                break;
        }
        ans[j] = cur++;
        ans[i] = cur++;
        seg.update(p[i], INF);
        seg.update(p[j], INF);
        bit.update(p[i], -1);
        bit.update(p[j], -1);
    }

    for (int x: ans) cout << x << " ";
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

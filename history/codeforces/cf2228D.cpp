#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct SegTree01 {
    int n = 0;
    vector<int> tree; // segment sum over 0/1 entries

    SegTree01() = default;
    explicit SegTree01(int n_) { init(n_); }

    void init(int n_) {
        n = n_;
        tree.assign(4 * max(1, n), 0);
    }

    // Set slot `pos` to value `v` (v should be 0 or 1).
    void set_val(int pos, int v) { set_val(1, 0, n - 1, pos, v); }

    // Convenience wrappers.
    void insert(int pos) { set_val(pos, 1); }
    void erase(int pos) { set_val(pos, 0); }

    // Return first index with value 1. Return -1 if none exists.
    int first_one() const {
        if (n == 0 || tree[1] == 0) return -1;
        return first_one(1, 0, n - 1);
    }

    // Return last index with value 1. Return -1 if none exists.
    int last_one() const {
        if (n == 0 || tree[1] == 0) return -1;
        return last_one(1, 0, n - 1);
    }

private:
    void set_val(int p, int l, int r, int pos, int v) {
        if (l == r) {
            tree[p] = v;
            return;
        }
        int m = (l + r) >> 1;
        if (pos <= m)
            set_val(p << 1, l, m, pos, v);
        else
            set_val(p << 1 | 1, m + 1, r, pos, v);
        tree[p] = tree[p << 1] + tree[p << 1 | 1];
    }

    int first_one(int p, int l, int r) const {
        if (l == r) return l;
        int lc = p << 1;
        int m = (l + r) >> 1;
        if (tree[lc] > 0) return first_one(lc, l, m);
        return first_one(lc | 1, m + 1, r);
    }

    int last_one(int p, int l, int r) const {
        if (l == r) return l;
        int rc = p << 1 | 1;
        int m = (l + r) >> 1;
        if (tree[rc] > 0) return last_one(rc, m + 1, r);
        return last_one(rc ^ 1, l, m);
    }
};


void solve() {
    int n;
    cin >> n;
    vector<int> xs(n), ys(n);
    map<int, vector<int>> y_to_xs;
    for (int i = 0; i < n; i++) {
        cin >> xs[i] >> ys[i];
        y_to_xs[ys[i]].push_back(xs[i]);
    }
    auto yy = ys;
    sort(yy.begin(), yy.end());
    yy.erase(unique(yy.begin(), yy.end()), yy.end());
    auto xx = xs;
    sort(xx.begin(), xx.end());
    xx.erase(unique(xx.begin(), xx.end()), xx.end());

    auto get_i = [&](int x) { return lower_bound(xx.begin(), xx.end(), x) - xx.begin(); };

    long long ans = 0;
    map<int, int> cnt[2];
    SegTree01 seg[2] = {SegTree01(n), SegTree01(n)};

    enum { LEFT = 0, RIGHT = 1 };

    auto insert = [&](int x, int dir) {
        int xid = get_i(x);
        if (++cnt[dir][xid] == 1) {
            seg[dir].insert(xid);
        }
    };
    auto erase = [&](int x, int dir) {
        int xid = get_i(x);
        if (--cnt[dir][xid] == 0) {
            seg[dir].erase(xid);
        }
    };
    for (int y: yy) {
        for (int x: y_to_xs[y]) {
            insert(x, RIGHT);
        }
    }

    for (int y: yy) {
        for (int x: y_to_xs[y]) {
            erase(x, RIGHT);
            insert(x, LEFT);
        }
        int lo = max(seg[LEFT].first_one(), seg[RIGHT].first_one());
        int hi = min(seg[LEFT].last_one(), seg[RIGHT].last_one());
        if (lo < hi) {
            ans += (hi - lo);
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

template<typename T>
concept IsInfo = requires(T a, T b) {
    { a + b } -> same_as<T>;
};

template<typename T, typename InfoType>
concept IsTag = requires(T t1, T t2, InfoType &info, int len) {
    { t1.apply_tag(t2) } -> same_as<void>;
    { t1.apply_to_info(info, len) } -> same_as<void>;
    { t1.advanced(len, len) } -> same_as<T>;
    { t1.has_lazy } -> convertible_to<bool>;
};

template<typename InfoT, typename TagT>
    requires IsInfo<InfoT> && IsTag<TagT, InfoT>
class LazySegmentTree {
private:
    static constexpr int lc(int p) { return 2 * p + 1; }
    static constexpr int rc(int p) { return 2 * p + 2; }

    int n;
    vector<InfoT> tree;
    vector<TagT> lazy;

    void push_up(int p) { tree[p] = tree[lc(p)] + tree[rc(p)]; }

    void push_down(int p, int l, int r) {
        if (!lazy[p].has_lazy) return;
        int m = l + (r - l) / 2;

        TagT left_tag = lazy[p].advanced(0, r - m);
        lazy[lc(p)].apply_tag(left_tag);
        left_tag.apply_to_info(tree[lc(p)], m - l);

        TagT right_tag = lazy[p].advanced(m - l, 0);
        lazy[rc(p)].apply_tag(right_tag);
        right_tag.apply_to_info(tree[rc(p)], r - m);

        lazy[p] = TagT();
    }

    void modify(int p, int l, int r, int ql, int qr, const TagT &v) {
        if (ql <= l && r <= qr) {
            TagT cur_tag = v.advanced(l - ql, qr - r);
            cur_tag.apply_to_info(tree[p], r - l);
            lazy[p].apply_tag(cur_tag);
            return;
        }
        push_down(p, l, r);
        int m = l + (r - l) / 2;
        if (ql < m) modify(lc(p), l, m, ql, qr, v);
        if (qr > m) modify(rc(p), m, r, ql, qr, v);
        push_up(p);
    }

    InfoT query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[p];
        push_down(p, l, r);
        int m = l + (r - l) / 2;
        if (qr <= m) return query(lc(p), l, m, ql, qr);
        if (ql >= m) return query(rc(p), m, r, ql, qr);
        return query(lc(p), l, m, ql, qr) + query(rc(p), m, r, ql, qr);
    }

    template<typename T>
    void build(int p, int l, int r, const vector<T> &init_val) {
        if (r - l == 1) {
            tree[p] = InfoT{init_val[l]};
            return;
        }
        int m = l + (r - l) / 2;
        build(lc(p), l, m, init_val);
        build(rc(p), m, r, init_val);
        push_up(p);
    }

public:
    LazySegmentTree(int n) : n(n), tree(4 * n), lazy(4 * n) {}

    template<typename T>
    LazySegmentTree(const vector<T> &init_val) : n(init_val.size()), tree(4 * n), lazy(4 * n) {
        build(0, 0, n, init_val);
    }

    void modify(int ql, int qr, const TagT &v) {
        if (ql >= qr || ql < 0 || qr > n) [[unlikely]] {
            assert(false && "SegmentTree: Query range out of bounds or invalid!");
            return;
        }
        modify(0, 0, n, ql, qr, v);
    }

    InfoT query(int ql, int qr) {
        if (ql >= qr || ql < 0 || qr > n) [[unlikely]] {
            assert(false && "SegmentTree: Query range out of bounds or invalid!");
            return InfoT{};
        }
        return query(0, 0, n, ql, qr);
    }
};

struct Info {
    int mx = 0;
    Info(int val = 0) : mx(val) {}
    friend Info operator+(const Info &lhs, const Info &rhs) { return Info{max(lhs.mx, rhs.mx)}; }
};

struct Tag {
    int inc = 0;
    bool has_lazy = false;

    void apply_tag(const Tag &new_tag) {
        if (!new_tag.has_lazy) return;
        inc += new_tag.inc;
        has_lazy = true;
    }

    void apply_to_info(Info &info, int len) const {
        (void) len;
        if (!has_lazy) return;
        info.mx += inc;
    }

    Tag advanced(int dl, int dr) const {
        (void) dl, (void) dr;
        return *this;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<int> p(2 * n + 1);
    vector<vector<int>> pos(n + 2);
    for (int i = 1; i <= n; i++) {
        cin >> p[i];
        p[i + n] = p[i];
    }
    for (int i = 1; i <= 2 * n; i++) {
        pos[p[i]].push_back(i);
    }

    LazySegmentTree<Info, Tag> seg(2 * n + 1);
    vector<int> lst(n + 2, 0);
    int cnt = 0;
    for (int i = 1; i <= 2 * n; i++) {
        int x = p[i];
        cnt++;
        if (lst[x - 1]) cnt--;
        if (lst[x + 1]) cnt--;
        seg.modify(i, i + 1, Tag{cnt, true});
        lst[x] = i;
    }

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (seg.query(i, i + n).mx <= 2) ans++;
        int x = p[i];
        seg.modify(i + 1, 2 * n + 1, Tag{-1, true});
        for (int y: {x - 1, x + 1}) {
            auto it = upper_bound(pos[y].begin(), pos[y].end(), i);
            if (it != pos[y].end()) {
                seg.modify(*it, 2 * n + 1, Tag{1, true});
            }
        }
    }
    cout << ans << "\n";
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

constexpr int MOD = 1'000'000'009;

template<size_t Rows, size_t Cols>
struct Matrix {
    array<array<int, Cols>, Rows> a;
    Matrix() : a{} {}
    Matrix(const array<array<int, Cols>, Rows> &arr) : a(arr) {}

    template<size_t C2>
    friend Matrix<Rows, C2> operator*(const Matrix<Rows, Cols> &lhs, const Matrix<Cols, C2> &rhs) {
        Matrix<Rows, C2> ret;
        for (size_t i = 0; i < Rows; ++i) {
            for (size_t k = 0; k < Cols; ++k) {
                if (lhs.a[i][k] == 0) continue;
                long long factor = lhs.a[i][k];
                for (size_t j = 0; j < C2; ++j) {
                    (ret.a[i][j] += factor * rhs.a[k][j] % MOD) %= MOD;
                }
            }
        }
        return ret;
    }
};

// ============================= Generic template ==============================
// Matrix, F_pow/init_matrix, the LazySegmentTree, and the IsInfo/IsTag concepts
// below are problem-agnostic. Only Info/Tag and solve() need to change per task.
// ===============================================================================

// F_pow[k] is the transition matrix raised to the k-th power; row 0 of
// status * F_pow[k] gives (a_k, b_k, sum_{i<k} a_i) for a sequence started at
// (a_0, b_0) = (a, b) evolving as a_{i+1} = b_i, b_{i+1} = a_i + b_i.
vector<Matrix<3, 3>> F_pow{
        array<array<int, 3>, 3>{
                array<int, 3>{1, 0, 0},
                array<int, 3>{0, 1, 0},
                array<int, 3>{0, 0, 1},
        },
        array<array<int, 3>, 3>{
                array<int, 3>{0, 1, 1},
                array<int, 3>{1, 1, 0},
                array<int, 3>{0, 0, 1},
        },
};

// Ensures F_pow[0..max_len] are available.
void init_matrix(int max_len) {
    if (max_len + 1 <= static_cast<int>(F_pow.size())) return;
    F_pow.reserve(max_len + 1);
    while (static_cast<int>(F_pow.size()) <= max_len) {
        F_pow.push_back(F_pow.back() * F_pow[1]);
    }
}

// example: cf446C
template<typename T>
concept IsInfo = requires(T a, T b) {
    { a + b } -> same_as<T>;
};

template<typename T, typename InfoType>
concept IsTag = requires(T t1, T t2, InfoType &info, int len) {
    { t1.apply_tag(t2) } -> same_as<void>;
    { t1.apply_to_info(info, len) } -> same_as<void>;
    // advanced(dl, dr): re-anchor to a sub-interval `dl` in from the left edge
    // and `dr` in from the right edge; a one-sided tag ignores whichever it
    // doesn't need.
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
            tree[p] = InfoT{init_val[l] % MOD};
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

// ============================ Problem-specific ================================
// Info/Tag encode this task: range-add a Fibonacci-like sequence, range-sum
// query. Swap these two types (and their concept-required members) to reuse
// the LazySegmentTree above for a different problem.
// ===============================================================================

struct Info {
    int mx = 0;
    friend Info operator+(const Info &lhs, const Info &rhs) { return Info{(lhs.mx + rhs.mx) % MOD}; }
};

using Status = Matrix<1, 3>;

struct Tag {
    int a = 0, b = 0;
    bool has_lazy = false;

    void apply_tag(const Tag &new_tag) {
        if (!new_tag.has_lazy) return;
        (a += new_tag.a) %= MOD;
        (b += new_tag.b) %= MOD;
        has_lazy = true;
    }

    void apply_to_info(Info &info, int len) const {
        if (!has_lazy) return;
        auto res = state() * F_pow[len];
        (info.mx += res.a[0][2]) %= MOD;
    }

    // Re-anchors the tag to a sub-interval whose left edge is `dl` further from
    // this tag's left edge and whose right edge is `dr` short of this tag's
    // right edge. This tag is left-anchored, so only `dl` matters; a
    // right-anchored tag would instead advance on `dr` and ignore `dl`.
    Tag advanced(int dl, int dr) const {
        (void) dr;
        if (!has_lazy) return *this;
        auto res = state() * F_pow[dl];
        return Tag{res.a[0][0], res.a[0][1], true};
    }

private:
    Status state() const {
        Status s;
        s.a[0] = {a, b, 0};
        return s;
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    init_matrix(n + 2);
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    LazySegmentTree<Info, Tag> seg(a);

    while (m--) {
        int type, l, r;
        cin >> type >> l >> r;

        int ql = l - 1;
        int qr = r;

        if (type == 1) {
            seg.modify(ql, qr, Tag{1, 1, true});
        } else {
            cout << seg.query(ql, qr).mx << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
#ifdef LOCAL
    cin >> t;
#endif
    while (t--) {
        solve();
    }
}

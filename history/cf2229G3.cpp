#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

namespace rgs = std::ranges;

constexpr i64 inf = 4E18;
template<class Info, class Tag>
struct LazySegmentTree {
    int n;
    std::vector<Info> info;
    std::vector<Tag> tag;
    LazySegmentTree() : n(0) {}
    LazySegmentTree(int n_, Info v_ = Info()) { init(n_, v_); }
    template<class T>
    LazySegmentTree(std::vector<T> init_) {
        init(init_);
    }
    void init(int n_, Info v_ = Info()) { init(std::vector(n_, v_)); }
    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        info.assign(4 << (std::bit_width(unsigned(n)) - 1), Info());
        tag.assign(4 << (std::bit_width(unsigned(n)) - 1), Tag());
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }
    void pull(int p) { info[p] = info[2 * p] + info[2 * p + 1]; }
    void apply(int p, const Tag &v) {
        info[p].apply(v);
        tag[p].apply(v);
    }
    void push(int p) {
        apply(2 * p, tag[p]);
        apply(2 * p + 1, tag[p]);
        tag[p] = Tag();
    }
    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        push(p);
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }
    void modify(int p, const Info &v) { modify(1, 0, n, p, v); }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) {
            return Info();
        }
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        push(p);
        return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
    }
    Info rangeQuery(int l, int r) { return rangeQuery(1, 0, n, l, r); }
    void rangeApply(int p, int l, int r, int x, int y, const Tag &v) {
        if (l >= y || r <= x) {
            return;
        }
        if (l >= x && r <= y) {
            apply(p, v);
            return;
        }
        int m = (l + r) / 2;
        push(p);
        rangeApply(2 * p, l, m, x, y, v);
        rangeApply(2 * p + 1, m, r, x, y, v);
        pull(p);
    }
    void rangeApply(int l, int r, const Tag &v) { return rangeApply(1, 0, n, l, r, v); }
    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F &&pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        push(p);
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(2 * p + 1, m, r, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findFirst(int l, int r, F &&pred) {
        return findFirst(1, 0, n, l, r, pred);
    }
    template<class F>
    int findLast(int p, int l, int r, int x, int y, F &&pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        push(p);
        int res = findLast(2 * p + 1, m, r, x, y, pred);
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findLast(int l, int r, F &&pred) {
        return findLast(1, 0, n, l, r, pred);
    }

    void advance(int p, int l, int r, i64 t) {
        if (t < info[p].t1 && t < info[p].t2) {
            return apply(p, {t});
        }
        int m = (l + r) / 2;
        push(p);
        advance(2 * p, l, m, t);
        advance(2 * p + 1, m, r, t);
        pull(p);
    }

    void advance(i64 t) { advance(1, 0, n, t); }
};

struct Tag {
    i64 adv = 0;
    void apply(const Tag &t) { adv += t.adv; }
};

struct Info {
    i64 b1 = -inf;
    i64 k1 = 0;
    i64 t1 = inf;

    i64 b2 = -inf;
    i64 k2 = 0;
    i64 t2 = inf;

    void apply(const Tag &t) {
        b1 += k1 * t.adv;
        b2 += k2 * t.adv;
        t1 -= t.adv;
        t2 -= t.adv;
    }
};

Info operator+(Info l, Info r) {
    Info x;
    x.t1 = std::min(l.t1, r.t1);
    x.t2 = std::min(l.t2, r.t2);

    if (std::make_pair(l.b1, l.k1) < std::make_pair(r.b1, r.k1)) {
        std::swap(l.b1, r.b1);
        std::swap(l.k1, r.k1);
    }
    if (std::make_pair(l.b2, l.k2) < std::make_pair(r.b2, r.k2)) {
        std::swap(l.b2, r.b2);
        std::swap(l.k2, r.k2);
    }

    x.k1 = l.k1;
    x.b1 = l.b1;
    x.k2 = l.k2;
    x.b2 = l.b2;

    if (r.k1 > l.k1) {
        x.t1 = std::min(x.t1, (l.b1 - r.b1 + r.k1 - l.k1 - 1) / (r.k1 - l.k1));
    }
    if (r.k2 > l.k2) {
        x.t2 = std::min(x.t2, (l.b2 - r.b2 + r.k2 - l.k2 - 1) / (r.k2 - l.k2));
    }

    return x;
}

void solve() {
    int n, k, x;
    std::cin >> n >> k >> x, x--;
    std::vector<int> h(n);
    for (int i = 0; i < n; i++) std::cin >> h[i];
    std::vector<int> d(n - 1);
    for (int i = 0; i < n - 1; i++) std::cin >> d[i];
    std::vector<int> t(n);
    for (int i = x - 1; i >= 0; i--) t[i] = std::max(t[i + 1] + 1, d[i]);
    for (int i = x + 1; i < n; i++) t[i] = std::max(t[i - 1] + 1, d[i - 1]);
    std::vector<i64> dp(n, -inf);
    dp[x] = 0;
    std::vector<i64> pre(n + 1);
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + h[i];
    LazySegmentTree<Info, Tag> seg(n);
    std::vector<std::array<int, 2>> p;
    for (int i = 0; i < n; i++) {
        if (i != x) {
            p.push_back({t[i] + (i - x), i});
            p.push_back({t[i] - (i - x), i});
        }
    }

    rgs::sort(p, [&](auto a, auto b) {
        if (a[0] != b[0]) {
            return a[0] < b[0];
        }
        bool fa = (a[1] < x) == (a[0] == t[a[1]] - (a[1] - x));
        bool fb = (b[1] < x) == (b[0] == t[b[1]] - (b[1] - x));
        if (fa != fb) {
            return fa;
        }
        return std::abs(a[1] - x) < std::abs(b[1] - x);
    });

    seg.modify(x, {.b1 = pre[x], .k1 = h[x], .b2 = -pre[x + 1], .k2 = h[x]});

    int lst = 0;
    for (auto [tm, i]: p) {
        seg.advance(tm - lst);
        lst = tm;

        if (i < x) {
            if (tm == t[i] + (i - x)) {
                dp[i] = std::max(dp[i], seg.rangeQuery(i + 1, n).b1 - pre[i]);
                auto res = seg.rangeQuery(i, i + 1);
                res.b1 = dp[i] + pre[i];
                res.k1 = h[i];
                seg.modify(i, res);
            } else {
                auto res = seg.rangeQuery(i, i + 1);
                res.b2 = dp[i] - pre[i + 1];
                res.k2 = h[i];
                seg.modify(i, res);
            }
        } else {
            if (tm == t[i] - (i - x)) {
                dp[i] = std::max(dp[i], seg.rangeQuery(0, i).b2 + pre[i + 1]);
                auto res = seg.rangeQuery(i, i + 1);
                res.b2 = dp[i] - pre[i + 1];
                res.k2 = h[i];
                seg.modify(i, res);
            } else {
                auto res = seg.rangeQuery(i, i + 1);
                res.b1 = dp[i] + pre[i];
                res.k1 = h[i];
                seg.modify(i, res);
            }
        }
    }

    i64 ans = 0;
    for (int i = 0; i < n; i++) {
        if (t[i] <= k) {
            ans = std::max(ans, dp[i] + i64(h[i]) * (k - t[i]));
        }
    }
    std::cout << ans << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}

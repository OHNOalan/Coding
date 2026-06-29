#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;


template<class T>
constexpr T power(T a, u64 b, T res = 1) {
    for (; b != 0; b /= 2, a *= a) {
        if (b & 1) {
            res *= a;
        }
    }
    return res;
}

template<u32 P>
constexpr u32 mulMod(u32 a, u32 b) {
    return u64(a) * b % P;
}

template<u64 P>
constexpr u64 mulMod(u64 a, u64 b) {
    u64 res = a * b - u64(1.L * a * b / P - 0.5L) * P;
    res %= P;
    return res;
}

constexpr i64 safeMod(i64 x, i64 m) {
    x %= m;
    if (x < 0) {
        x += m;
    }
    return x;
}

constexpr std::pair<i64, i64> invGcd(i64 a, i64 b) {
    a = safeMod(a, b);
    if (a == 0) {
        return {b, 0};
    }

    i64 s = b, t = a;
    i64 m0 = 0, m1 = 1;

    while (t) {
        i64 u = s / t;
        s -= t * u;
        m0 -= m1 * u;

        std::swap(s, t);
        std::swap(m0, m1);
    }

    if (m0 < 0) {
        m0 += b / s;
    }

    return {s, m0};
}

template<std::unsigned_integral U, U P>
struct ModIntBase {
public:
    constexpr ModIntBase() : x(0) {}
    template<std::unsigned_integral T>
    constexpr ModIntBase(T x_) : x(x_ % mod()) {}
    template<std::signed_integral T>
    constexpr ModIntBase(T x_) {
        using S = std::make_signed_t<U>;
        S v = x_ % S(mod());
        if (v < 0) {
            v += mod();
        }
        x = v;
    }

    constexpr static U mod() { return P; }

    constexpr U val() const { return x; }

    constexpr ModIntBase operator-() const {
        ModIntBase res;
        res.x = (x == 0 ? 0 : mod() - x);
        return res;
    }

    constexpr ModIntBase inv() const { return power(*this, mod() - 2); }

    constexpr ModIntBase &operator*=(const ModIntBase &rhs) & {
        x = mulMod<mod()>(x, rhs.val());
        return *this;
    }
    constexpr ModIntBase &operator+=(const ModIntBase &rhs) & {
        x += rhs.val();
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr ModIntBase &operator-=(const ModIntBase &rhs) & {
        x -= rhs.val();
        if (x >= mod()) {
            x += mod();
        }
        return *this;
    }
    constexpr ModIntBase &operator/=(const ModIntBase &rhs) & { return *this *= rhs.inv(); }

    friend constexpr ModIntBase operator*(ModIntBase lhs, const ModIntBase &rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator+(ModIntBase lhs, const ModIntBase &rhs) {
        lhs += rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator-(ModIntBase lhs, const ModIntBase &rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator/(ModIntBase lhs, const ModIntBase &rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend constexpr std::istream &operator>>(std::istream &is, ModIntBase &a) {
        i64 i;
        is >> i;
        a = i;
        return is;
    }
    friend constexpr std::ostream &operator<<(std::ostream &os, const ModIntBase &a) { return os << a.val(); }

    friend constexpr bool operator==(const ModIntBase &lhs, const ModIntBase &rhs) { return lhs.val() == rhs.val(); }
    friend constexpr std::strong_ordering operator<=>(const ModIntBase &lhs, const ModIntBase &rhs) {
        return lhs.val() <=> rhs.val();
    }

private:
    U x;
};

template<u32 P>
using ModInt = ModIntBase<u32, P>;
template<u64 P>
using ModInt64 = ModIntBase<u64, P>;

struct Barrett {
public:
    Barrett(u32 m_) : m(m_), im((u64) (-1) / m_ + 1) {}

    constexpr u32 mod() const { return m; }

    constexpr u32 mul(u32 a, u32 b) const {
        u64 z = a;
        z *= b;

        u64 x = u64((u128(z) * im) >> 64);

        u32 v = u32(z - x * m);
        if (m <= v) {
            v += m;
        }
        return v;
    }

private:
    u32 m;
    u64 im;
};

template<u32 Id>
struct DynModInt {
public:
    constexpr DynModInt() : x(0) {}
    template<std::unsigned_integral T>
    constexpr DynModInt(T x_) : x(x_ % mod()) {}
    template<std::signed_integral T>
    constexpr DynModInt(T x_) {
        int v = x_ % int(mod());
        if (v < 0) {
            v += mod();
        }
        x = v;
    }

    constexpr static void setMod(u32 m) { bt = m; }

    static u32 mod() { return bt.mod(); }

    constexpr u32 val() const { return x; }

    constexpr DynModInt operator-() const {
        DynModInt res;
        res.x = (x == 0 ? 0 : mod() - x);
        return res;
    }

    constexpr DynModInt inv() const {
        auto v = invGcd(x, mod());
        assert(v.first == 1);
        return v.second;
    }

    constexpr DynModInt &operator*=(const DynModInt &rhs) & {
        x = bt.mul(x, rhs.val());
        return *this;
    }
    constexpr DynModInt &operator+=(const DynModInt &rhs) & {
        x += rhs.val();
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr DynModInt &operator-=(const DynModInt &rhs) & {
        x -= rhs.val();
        if (x >= mod()) {
            x += mod();
        }
        return *this;
    }
    constexpr DynModInt &operator/=(const DynModInt &rhs) & { return *this *= rhs.inv(); }

    friend constexpr DynModInt operator*(DynModInt lhs, const DynModInt &rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr DynModInt operator+(DynModInt lhs, const DynModInt &rhs) {
        lhs += rhs;
        return lhs;
    }
    friend constexpr DynModInt operator-(DynModInt lhs, const DynModInt &rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr DynModInt operator/(DynModInt lhs, const DynModInt &rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend constexpr std::istream &operator>>(std::istream &is, DynModInt &a) {
        i64 i;
        is >> i;
        a = i;
        return is;
    }
    friend constexpr std::ostream &operator<<(std::ostream &os, const DynModInt &a) { return os << a.val(); }

    friend constexpr bool operator==(const DynModInt &lhs, const DynModInt &rhs) { return lhs.val() == rhs.val(); }
    friend constexpr std::strong_ordering operator<=>(const DynModInt &lhs, const DynModInt &rhs) {
        return lhs.val() <=> rhs.val();
    }

private:
    u32 x;
    static Barrett bt;
};

template<u32 Id>
Barrett DynModInt<Id>::bt = 998244353;

using Z = ModInt<998244353>;

struct Comb {
    int n;
    std::vector<Z> _fac;
    std::vector<Z> _invfac;
    std::vector<Z> _inv;

    Comb() : n{0}, _fac{1}, _invfac{1}, _inv{0} {}
    Comb(int n) : Comb() { init(n); }

    void init(int m) {
        if (m <= n) return;
        _fac.resize(m + 1);
        _invfac.resize(m + 1);
        _inv.resize(m + 1);

        for (int i = n + 1; i <= m; i++) {
            _fac[i] = _fac[i - 1] * i;
        }
        _invfac[m] = _fac[m].inv();
        for (int i = m; i > n; i--) {
            _invfac[i - 1] = _invfac[i] * i;
            _inv[i] = _invfac[i] * _fac[i - 1];
        }
        n = m;
    }

    Z fac(int m) {
        if (m > n) init(2 * m);
        return _fac[m];
    }
    Z invfac(int m) {
        if (m > n) init(2 * m);
        return _invfac[m];
    }
    Z inv(int m) {
        if (m > n) init(2 * m);
        return _inv[m];
    }
    Z binom(int n, int m) {
        if (n < m || m < 0) return 0;
        return fac(n) * invfac(m) * invfac(n - m);
    }
} comb;

namespace atcoder {

    namespace internal {

        // @param n `0 <= n`
        // @return minimum non-negative `x` s.t. `n <= 2**x`
        int ceil_pow2(int n) {
            int x = 0;
            while ((1U << x) < (unsigned int) (n)) x++;
            return x;
        }

        // @param n `1 <= n`
        // @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
        int bsf(unsigned int n) {
#ifdef _MSC_VER
            unsigned long index;
            _BitScanForward(&index, n);
            return index;
#else
            return countr_zero(n);
#endif
        }

    } // namespace internal

    template<class S, S (*op)(const S &, const S &), S (*e)()>
    struct segtree {
    public:
        segtree() : segtree(0) {}
        segtree(int n) : segtree(std::vector<S>(n, e())) {}
        segtree(const std::vector<S> &v) : _n(int(v.size())) {
            log = internal::ceil_pow2(_n);
            size = 1 << log;
            d = std::vector<S>(2 * size, e());
            for (int i = 0; i < _n; i++) d[size + i] = v[i];
            for (int i = size - 1; i >= 1; i--) {
                update(i);
            }
        }

        void set(int p, S x) {
            assert(0 <= p && p < _n);
            p += size;
            d[p] = x;
            for (int i = 1; i <= log; i++) update(p >> i);
        }

        S get(int p) {
            assert(0 <= p && p < _n);
            return d[p + size];
        }

        S prod(int l, int r) {
            assert(0 <= l && l <= r && r <= _n);
            S sml = e(), smr = e();
            l += size;
            r += size;

            while (l < r) {
                if (l & 1) sml = op(sml, d[l++]);
                if (r & 1) smr = op(d[--r], smr);
                l >>= 1;
                r >>= 1;
            }
            return op(sml, smr);
        }

        S all_prod() { return d[1]; }

        template<bool (*f)(S)>
        int max_right(int l) {
            return max_right(l, [](S x) { return f(x); });
        }
        template<class F>
        int max_right(int l, F f) {
            assert(0 <= l && l <= _n);
            assert(f(e()));
            if (l == _n) return _n;
            l += size;
            S sm = e();
            do {
                while (l % 2 == 0) l >>= 1;
                if (!f(op(sm, d[l]))) {
                    while (l < size) {
                        l = (2 * l);
                        if (f(op(sm, d[l]))) {
                            sm = op(sm, d[l]);
                            l++;
                        }
                    }
                    return l - size;
                }
                sm = op(sm, d[l]);
                l++;
            } while ((l & -l) != l);
            return _n;
        }

        template<bool (*f)(S)>
        int min_left(int r) {
            return min_left(r, [](S x) { return f(x); });
        }
        template<class F>
        int min_left(int r, F f) {
            assert(0 <= r && r <= _n);
            assert(f(e()));
            if (r == 0) return 0;
            r += size;
            S sm = e();
            do {
                r--;
                while (r > 1 && (r % 2)) r >>= 1;
                if (!f(op(d[r], sm))) {
                    while (r < size) {
                        r = (2 * r + 1);
                        if (f(op(d[r], sm))) {
                            sm = op(d[r], sm);
                            r--;
                        }
                    }
                    return r + 1 - size;
                }
                sm = op(d[r], sm);
            } while ((r & -r) != r);
            return 0;
        }

    private:
        int _n, size, log;
        std::vector<S> d;

        void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
    };

} // namespace atcoder

struct Node {
    Z n = 0;
    long long real_sum = 0; // 改用 long long 维护不取模的真实总和
    Z k = 0;
    Z sumA = 0;
    Z sumT = 0;
    Z sumA2 = 0;
    Z sumAT = 0;
    Z sumT2 = 0;
    Node() {}
    Node(int v) :
        n(1), real_sum(v >= 0 ? v : 0), k(v < 0 ? 1 : 0), sumA(v >= 0 ? v : 0), sumT(k), sumA2(sumA * sumA),
        sumAT(sumA * sumT), sumT2(sumT * sumT) {}
};

Node e() { return Node(); }
Node op(const Node &x, const Node &y) {
    Node r;
    r.n = x.n + y.n;
    r.real_sum = x.real_sum + y.real_sum; // 真实和直接累加，绝不取模
    r.k = x.k + y.k;
    Z x_sum_Z = x.real_sum;
    r.sumA = x.sumA + y.sumA + y.n * x_sum_Z;
    r.sumT = x.sumT + y.sumT + y.n * x.k;
    r.sumA2 = x.sumA2 + y.sumA2 + 2 * x_sum_Z * y.sumA + y.n * x_sum_Z * x_sum_Z;
    r.sumAT = x.sumAT + y.sumAT + y.n * x_sum_Z * x.k + x_sum_Z * y.sumT + y.sumA * x.k;
    r.sumT2 = x.sumT2 + y.sumT2 + 2 * x.k * y.sumT + y.n * x.k * x.k;
    return r;
}

const int N = 3e5;
const int K = 1e6;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    vector<Node> init(n);
    for (int i = 0; i < n; i++) init[i] = Node(a[i]);
    atcoder::segtree<Node, op, e> seg(init);
    int op, p, v, l, r, m;
    while (q--) {
        cin >> op;
        if (op == 1) {
            cin >> p >> v;
            --p;
            seg.set(p, Node(v));
        } else {
            cin >> l >> r >> m;
            --l;
            Node x = seg.prod(l, r);
            long long remain = m - x.real_sum;
            if (remain < 0) {
                cout << 0 << '\n';
                continue;
            }
            int kCnt = int(x.k.val());
            if (kCnt == 0) {
                if (remain == 0) {
                    cout << x.sumA2.val() << '\n';
                } else {
                    void(cout << 0 << '\n');
                }
                continue;
            }

            Z freeCnt = remain;
            Z invK = comb.inv(kCnt);
            Z invK1 = comb.inv(kCnt + 1);
            Z avg = freeCnt * invK;
            Z E_xp_xq = freeCnt * (freeCnt - 1) * invK * invK1;
            Z E_xp2 = 2 * E_xp_xq + avg;

            Z ans_inside = x.sumA2 + 2 * avg * x.sumAT + x.sumT * (E_xp2 - E_xp_xq) + x.sumT2 * E_xp_xq;

            Z C = comb.binom(remain + kCnt - 1, kCnt - 1);
            Z ans = C * ans_inside;
            cout << ans.val() << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    comb.init(N + K - 1);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

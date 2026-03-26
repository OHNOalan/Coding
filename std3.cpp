#include <bits/stdc++.h>

#define forn(i, n) for (int i = 0; i < int(n); i++)

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

const int MOD = 998244353;

int add(int a, int b) {
    a += b;
    if (a >= MOD) a -= MOD;
    if (a < 0) a += MOD;
    return a;
}

int mul(int a, int b) { return a * 1ll * b % MOD; }

int binpow(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = mul(res, a);
        a = mul(a, a);
        b >>= 1;
    }
    return res;
}

int n, k;
vector<vector<int>> g;

struct dpar {
    vector<Z> dp;
    Z ml, ad;
    dpar() {
        ml = 1, ad = 0;
        dp.clear();
    }
    Z &operator[](int k) { return dp[max(0, int(dp.size()) - k - 1)]; }
    Z get(int k) {
        Z x = dp[max(0, int(dp.size()) - k - 1)];
        return x * ml + ad;
    }
};

vector<dpar> dp;
vector<int> fact;

void dfs(int v) {
    int bst = -1;
    vector<Z> val;
    for (int u: g[v]) {
        dfs(u);
        dp[u].dp.push_back(-dp[u].ad / dp[u].ml);
        if (bst == -1 || dp[u].dp.size() > dp[bst].dp.size()) bst = u;
        val.push_back(dp[u].get(k - 1));
    }
    if (val.empty()) {
        dp[v].dp.push_back(1);
        return;
    }

    vector<Z> pr(val.size() + 1, 1), su(val.size() + 1, 1);
    forn(i, val.size()) {
        pr[i + 1] = pr[i] * val[i];
        su[val.size() - i - 1] = su[val.size() - i] * val[val.size() - i - 1];
    }

    swap(dp[v], dp[bst]);
    {
        int i = find(g[v].begin(), g[v].end(), bst) - g[v].begin();
        Z cur = fact[g[v].size() - 1] * pr[i] * su[i + 1];
        if (cur == 0) {
            int mx = 1;
            for (int u: g[v]) mx = max(mx, int(dp[u].dp.size()));
            dp[v].dp.assign(mx, 0);
            dp[v].ml = 1, dp[v].ad = 0;
        } else {
            dp[v].ml = dp[v].ml * cur;
            dp[v].ad = dp[v].ad * cur;
        }
    }

    int rev = binpow(dp[v].ml.val(), MOD - 2);
    forn(i, g[v].size()) {
        int u = g[v][i];
        if (u == bst) continue;
        Z cur = fact[g[v].size() - 1] * pr[i] * su[i + 1];
        Z tot = cur * dp[u].get(n);
        dp[v].ad = dp[v].ad + tot;
        forn(j, dp[u].dp.size()) {
            dp[v][j] = dp[v][j] - rev * tot;
            dp[v][j] = dp[v][j] + cur * dp[u].get(j) * rev;
        }
    }
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--) {
        cin >> n >> k;
        g.assign(n, {});
        for (int i = 1; i < n; ++i) {
            int p;
            cin >> p;
            --p;
            g[p].push_back(i);
        }
        dp.assign(n, {});
        fact.resize(n + 1, 1);
        for (int i = 1; i <= n; ++i) fact[i] = mul(fact[i - 1], i);
        dfs(0);
        cout << dp[0].get(n) << '\n';
    }
    return 0;
}

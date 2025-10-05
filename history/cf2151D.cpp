#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int mod = 998244353;

struct mint {
    int x;

    mint() { x = 0; }
    mint(int32_t xx) {
        x = xx % mod;
        if (x < 0)
            x += mod;
    }
    mint(long long xx) {
        x = xx % mod;
        if (x < 0)
            x += mod;
    }

    int val() { return x; }
    mint &operator++() {
        x++;
        if (x == mod)
            x = 0;
        return *this;
    }
    mint &operator--() {
        if (x == 0)
            x = mod;
        x--;
        return *this;
    }
    mint operator++(int32_t) {
        mint result = *this;
        ++*this;
        return result;
    }

    mint operator--(int32_t) {
        mint result = *this;
        --*this;
        return result;
    }
    mint &operator+=(const mint &b) {
        x += b.x;
        if (x >= mod)
            x -= mod;
        return *this;
    }
    mint &operator-=(const mint &b) {
        x -= b.x;
        if (x < 0)
            x += mod;
        return *this;
    }
    mint &operator*=(const mint &b) {
        long long z = x;
        z *= b.x;
        z %= mod;
        x = (int) z;
        return *this;
    }
    mint operator+() const { return *this; }
    mint operator-() const { return mint() - *this; }
    mint operator/=(const mint &b) { return *this = *this * b.inv(); }
    mint power(long long n) const {
        mint ok = *this, r = 1;
        while (n) {
            if (n & 1) {
                r *= ok;
            }
            ok *= ok;
            n >>= 1;
        }
        return r;
    }
    mint inv() const { return power(mod - 2); }
    friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
    friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
    friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
    friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
    friend bool operator==(const mint &a, const mint &b) { return a.x == b.x; }
    friend bool operator!=(const mint &a, const mint &b) { return a.x != b.x; }
    mint power(mint a, long long n) { return a.power(n); }
    friend ostream &operator<<(ostream &os, const mint &m) {
        os << m.x;
        return os;
    }
    explicit operator bool() const { return x != 0; }
};

constexpr int N = 2e5 + 10;
mint ff[N], iff[N];

void init() {
    ff[0] = 1;
    for (int i = 1; i < N; i++) { ff[i] = ff[i - 1] * i; }
    iff[N - 1] = 1 / ff[N - 1];
    for (int i = N - 1; i; i--) { iff[i - 1] = iff[i] * i; }
    assert(iff[0] == 1);
    assert(iff[1] == 1);
}

mint C(int n, int r) {
    if (n == r)
        return mint(1);
    if (n < 0 || r < 0 || r > n)
        return mint(0);
    return ff[n] * iff[r] * iff[n - r];
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;

    int cur = 0;
    mint ans = 1;
    for (int i = n - 1; i >= 0; i--) {
        if (2 * i == n - 1)
            cur += 1;
        else if (2 * i < n)
            cur += 2;
        if (cur < a[i]) {
            cout << 0 << '\n';
            return;
        }

        ans *= C(cur, a[i]);
        cur -= a[i];
    }
    if (cur != 0)
        ans *= 0;

    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();
    int t;
    cin >> t;
    while (t--) solve();
}

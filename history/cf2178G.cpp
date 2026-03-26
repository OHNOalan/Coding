#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

template<typename T>
struct Fenwick {
    int n;
    std::vector<T> a;

    Fenwick(int n_ = 0) { init(n_); }

    void init(int n_) {
        n = n_;
        a.assign(n, T{});
    }

    void add(int x, const T &v) {
        for (int i = x + 1; i <= n; i += i & -i) {
            a[i - 1] = a[i - 1] + v;
        }
    }

    T sum(int x) {
        T ans{};
        for (int i = x; i > 0; i -= i & -i) {
            ans = ans + a[i - 1];
        }
        return ans;
    }

    T rangeSum(int l, int r) { return sum(r) - sum(l); }

    int select(const T &k) {
        int x = 0;
        T cur{};
        for (int i = bit_floor((unsigned) n); i; i /= 2) {
            if (x + i <= n && cur + a[x + i - 1] <= k) {
                x += i;
                cur = cur + a[x - 1];
            }
        }
        return x;
    }
};

struct Xor {
    u64 x;
    Xor(u64 x_ = 0) : x(x_) {}
};

Xor operator+(Xor a, Xor b) { return a.x ^ b.x; }

Xor operator-(Xor a, Xor b) { return a.x ^ b.x; }

std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

void solve() {
    int n;
    cin >> n;
    Fenwick<Xor> f0(2 * n), f1(2 * n);

    u64 sum = 0;
    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        u64 w = rng();
        u64 s0 = f0.rangeSum(a, b).x;
        u64 s1 = f1.rangeSum(a, b).x;
        u64 v0 = s0 ^ 1;
        u64 v1 = s1 ^ (v0 * w);
        sum ^= v1;
        f0.add(a, v0);
        f0.add(b, v0);
        f1.add(a, v1);
        f1.add(b, v1);
        cout << "01"[sum == 0];
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

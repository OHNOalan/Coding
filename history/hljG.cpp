#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using ud = long double;

const int MOD = 998244353;

#define DEBUG false

template<typename T>
void print(vector<T> &a, function<T(T x)> f = [](T x) { return x; }) {
    for(int i=0; i<a.size(); i++) {
        cout << f(a[i]) << " ";
    }
    cout << endl;
}

void print(set<int>& st) {
    for(int x : st) cout << x << " ";
    cout << endl;
}

void print(map<int, int>& mp) {
    for(auto [k, v] : mp) cout << k << " " << v << endl;
    cout << endl;
}

template<typename T>
T pow(T a, T b) {
    T ret = 1;
    while (b) {
        if (b & 1) {
            ret = (long long) ret * a % MOD;
        }
        a = (long long) a * a % MOD;
        b >>= 1;
    }
    return ret;
}

template<typename T>
T inv(T a) {
    return pow(a, MOD - 2);
}

const int MAXN = 1e5;
int fact[MAXN + 1], invfact[MAXN + 1];

void init() {
    fact[0] = invfact[0] = 1;
    for(int i=1; i<=MAXN; i++) {
        fact[i] = (ll) fact[i-1] * i % MOD;
        invfact[i] = (ll) invfact[i-1] * inv(i) % MOD;
    }
}

int C(int n, int m) {
    int tmp = (ll) fact[n] * invfact[m] % MOD;
    return (ll) tmp * invfact[n - m] % MOD;
}

ud cross_product(array<ud, 2> a, array<ud, 2> b) {
    return a[0] * b[1] - a[1] * b[0];
}

void solve() {
    int n; cin >> n;
    vector<array<ud,2>> points(n);
    array<ud, 2> sum = {0,0};
    for(int i=0; i<n; i++) {
        int x, y; cin >> x >> y;
        sum[0] += x, sum[1] += y;
        points[i][0] = x;
        points[i][1] = y;
    }
    vector<ud> prods(n);
    for(int i=0; i<n; i++) {
        prods[i] = cross_product(points[i], sum);
    }
    ranges::sort(prods);
    ud ans = 0, cur = 0;
    for(int i=1; i<n; i++) {
        cur += prods[n-i];
        ud area = cur / ((ll) 2 * i * (n - i));
        ans = max(ans, area);
    }
    cout << fixed << setprecision(10) << ans << '\n';
}

void check(int i) {
    // TODO
}

#define MULTICASE false

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    for(int i=0; i<t; i++) {
        solve();
    }
    // for(int i=0; i<t; i++) {
    //     check(i);
    // }
    return 0;
}

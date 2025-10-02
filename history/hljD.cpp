#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

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

void solve() {
    int n, m, k; cin >> n >> m >> k;
    vector<int> f(n + 1);
    f[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j=1; j<=min(k,i); j++) {
            int type = C(4 * j + m - 1, m-1);
            f[i] += (ll) f[i - j] * type % MOD;
            f[i] %= MOD;
        }
    }
    for(int i=1; i<n; i++) cout << f[i] << " "; cout << endl;
    for(int i=1; i<n; i++) cout << f[i] / f[i-1] << " "; cout << endl;
    cout << f[n] << '\n';
}

void check(int i) {
    // TODO
}

#define MULTICASE true

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

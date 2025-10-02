#include <bits/stdc++.h>
#define int long long
#define ll long long
#define pii pair<int,int>
#define piii pair<pii,pii>
#define fi first
#define se second
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
using namespace std;

const int MX = 2e5;
ll fact[MX + 1];
ll ifact[MX + 1];
ll MOD = 998244353;

ll binPow(ll base, ll exp) {
    ll ans = 1;
    while (exp) {
        if (exp % 2) {
            ans = (ans * base) % MOD;
        }
        base = (base * base) % MOD;
        exp /= 2;
    }
    return ans;
}

int nCk(int N, int K) {
    if (K > N || K < 0) {
        return 0;
    }
    return (fact[N] * ((ifact[K] * ifact[N - K]) % MOD)) % MOD;
}

void ICombo() {
    fact[0] = 1;
    for (int i = 1; i <= MX; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    ifact[MX] = binPow(fact[MX], MOD - 2);
    for (int i = MX - 1; i >= 0; i--) {
        ifact[i] = (ifact[i + 1] * (i + 1)) % MOD;
    }
}

void solve() {
    int n, ans = 0;
    cin >> n;
    vector<int> c(n);
    for (int r: c) {
        cin >> r;
        c[r]++;
    }
    vector<vector<int> > dp(n, vector<int>(1));
    vector<int> ps, co;
    for (int i = 1; i <= c[0]; i++) dp[0].push_back(nCk(c[0], i));
    for (int i = 1; i < n; i++) {
        ps.resize(1);
        co = ps;
        for (int r: dp[i - 1]) ps.push_back((ps.back() + r) % MOD);
        int m = ps.size() - 1;
        dp[i].resize(min(m, c[i] + 1));
        for (int j = 0; j <= c[i]; j++) co.push_back((co.back() + nCk(c[i], j)) % MOD);
        for (int j = 1; j < dp[i].size(); j++)
            dp[i][j] = nCk(c[i], j) * (ps[m] - ps[j] + MOD) % MOD + (co.back() - co[j + 1] + MOD) * dp[i - 1][j] % MOD;
    }
    int j = 0;
    for (auto r: dp) {
        n -= c[j++];
        for (int i = 1; i < r.size(); i++) (ans += i * r[i] % MOD * binPow(2, n)) %= MOD;
    }
    cout << ans << "\n";
}

int32_t main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    ICombo();
    int t = 1;
    cin >> t;
    while (t--) solve();
}

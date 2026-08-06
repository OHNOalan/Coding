#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;
const int MA = 500000; // 最小质因子筛范围
const int MF = 200000; // 阶乘/逆阶乘预处理上限

vector<int> spf(MA + 1);
vector<int> pr;
vector<long long> fact(MF + 1), inv_fact(MF + 1);

// 快速幂
long long qpow(long long a, long long e) {
    long long res = 1;
    while (e) {
        if (e & 1) res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return res;
}

// 预处理：最小质因子筛、阶乘、逆阶乘
void pre() {
    // 线性筛最小质因子
    for (int i = 2; i <= MA; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            pr.push_back(i);
        }
        for (int j = 0; j < pr.size() && pr[j] <= spf[i] && i * pr[j] <= MA; j++) {
            spf[i * pr[j]] = pr[j];
        }
    }
    // 阶乘
    fact[0] = 1;
    for (int i = 1; i <= MF; ++i) fact[i] = fact[i - 1] * i % MOD;
    // 逆阶乘
    inv_fact[MF] = qpow(fact[MF], MOD - 2);
    for (int i = MF - 1; i >= 0; --i) inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
}

// 组合数 C(n, r)
long long C(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n, x;
        cin >> n >> x;
        vector<pair<int, int>> fac;

        for (int i = 0; i < n; ++i) {
            int a;
            cin >> a;
            while (a > 1) {
                int p = spf[a];
                int c = 0;
                while (a % p == 0) {
                    a /= p;
                    c++;
                }
                fac.emplace_back(p, c);
            }
        }
        sort(fac.begin(), fac.end());


        vector<pair<int, int>> fx;
        int tx = x;
        while (tx > 1) {
            int p = spf[tx];
            int c = 0;
            while (tx % p == 0) {
                tx /= p;
                c++;
            }
            fx.emplace_back(p, c);
        }
        sort(fx.begin(), fx.end());

        const int MS = 40;
        vector<long long> ip(MS + 1);
        for (int s = 0; s <= MS; ++s) ip[s] = C(n + s - 1, s);


        auto mul_poly = [&](const vector<long long> &a, int k, int c, int sl) {
            vector<long long> b(sl + 1, 0);
            int mt = min(c, sl / k);
            vector<long long> ct(mt + 1);

            for (int t = 0; t <= mt; ++t) {
                long long bin = C(c, t);
                if (t & 1) bin = MOD - bin;
                ct[t] = bin;
            }

            for (int s = 0; s <= sl; ++s) {
                long long sum = 0;
                int lm = min(mt, s / k);
                for (int t = 0; t <= lm; ++t) sum = (sum + ct[t] * a[s - k * t]) % MOD;
                b[s] = sum;
            }
            return b;
        };


        auto get_cf = [&](const vector<long long> &a, int k, int c, int tg, int sl) {
            if (tg < 0 || tg > sl) return 0LL;
            int mt = min(c, tg / k);
            long long ret = 0;
            for (int t = 0; t <= mt; ++t) {
                long long tm = C(c, t);
                if (t & 1) tm = MOD - tm;
                ret = (ret + tm * a[tg - k * t]) % MOD;
            }
            return ret;
        };

        long long ans = 1;
        int p1 = 0, p2 = 0;
        int s1 = fac.size(), s2 = fx.size();


        while (p1 < s1 || p2 < s2) {
            int pr;
            if (p1 < s1 && p2 < s2)
                pr = min(fac[p1].first, fx[p2].first);
            else if (p1 < s1)
                pr = fac[p1].first;
            else
                pr = fx[p2].first;


            int f = 0;
            if (p2 < s2 && fx[p2].first == pr) f = fx[p2++].second;


            vector<int> es;
            while (p1 < s1 && fac[p1].first == pr) es.push_back(fac[p1++].second);

            if (es.empty() && f == 0) continue;

            if (es.empty() && f > 0) {
                ans = 0;
                break;
            }

            int me = *max_element(es.begin(), es.end());
            vector<int> cnt(me + 1, 0);
            for (int e: es) cnt[e]++;
            int c0 = n - (int) es.size();

            if (f == 0) {

                long long se = 0;
                for (int e: es) se += e;
                ans = ans * ((1 + se) % MOD) % MOD;
            } else {
                int sl = me + f;

                if (sl > MS) {
                    ip.resize(sl + 1);
                    for (int s = MS + 1; s <= sl; ++s) ip[s] = C(n + s - 1, s);
                }
                vector<long long> fp(ip.begin(), ip.begin() + sl + 1);
                vector<long long> fc = mul_poly(fp, 1, c0, sl);


                vector<int> sf(me + 2, 0);
                for (int v = me; v >= 1; --v) sf[v] = sf[v + 1] + cnt[v];

                long long ways = 0;
                for (int M = 1; M <= me; ++M) {
                    int cm = sf[M];
                    long long cm_val = get_cf(fc, M + 1, cm, M + f, sl);

                    int cp = (M == 1) ? c0 + cm : cnt[M - 1] + cm;
                    long long cp_val = get_cf(fp, M, cp, M + f, sl);

                    ways = (ways + cm_val - cp_val) % MOD;

                    if (M < me) {
                        vector<long long> fn = mul_poly(fc, M + 1, cnt[M], sl);
                        fp = move(fc);
                        fc = move(fn);
                    }
                }
                ways = (ways % MOD + MOD) % MOD;
                ans = ans * ways % MOD;
            }
        }
        cout << ans << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    pre();
    solve();
    return 0;
}

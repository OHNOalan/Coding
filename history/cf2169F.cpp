#include <bits/stdc++.h>

using namespace std;

#define fore(i, l, r) for (int i = int(l); i < int(r); i++)
#define sz(a) int((a).size())

const int LOGN = 19;
const int MOD = 998244353;
int g = 3;

int mul(int a, int b) { return int(a * 1ll * b % MOD); }
int norm(int a) {
    while (a >= MOD) a -= MOD;
    while (a < 0) a += MOD;
    return a;
}
int binPow(int a, int k) {
    int ans = 1;
    for (; k > 0; k >>= 1) {
        if (k & 1) ans = mul(ans, a);
        a = mul(a, a);
    }
    return ans;
}
int inv(int a) { return binPow(a, MOD - 2); }

vector<int> w[LOGN], rv;
bool precalced = false;

void precalc() {
    if (precalced) return;
    precalced = true;

    int wb = binPow(g, (MOD - 1) / (1 << LOGN));
    fore(st, 0, LOGN) {
        w[st].assign(1 << st, 1);

        int bw = binPow(wb, 1 << (LOGN - st - 1));
        int cw = 1;

        fore(k, 0, 1 << st) {
            w[st][k] = cw;
            cw = mul(cw, bw);
        }
    }

    rv.assign(1 << LOGN, 0);
    fore(i, 1, sz(rv)) rv[i] = (rv[i >> 1] >> 1) | ((i & 1) << (LOGN - 1));
}

const int MX = (1 << LOGN) + 3;

inline void fft(int a[MX], int n, bool inverse) {
    precalc();

    int ln = __builtin_ctz(n);
    assert((1 << ln) < MX);
    assert((1 << ln) == n);

    fore(i, 0, n) {
        int ni = rv[i] >> (LOGN - ln);
        if (i < ni) swap(a[i], a[ni]);
    }

    for (int st = 0; (1 << st) < n; st++) {
        int len = (1 << st);
        for (int k = 0; k < n; k += (len << 1)) {
            fore(pos, k, k + len) {
                int l = a[pos];
                int r = mul(a[pos + len], w[st][pos - k]);

                a[pos] = norm(l + r);
                a[pos + len] = norm(l - r);
            }
        }
    }

    if (inverse) {
        int in = inv(n);
        fore(i, 0, n) a[i] = mul(a[i], in);
        reverse(a + 1, a + n);
    }
}

int aa[MX], bb[MX], cc[MX];

vector<int> multiply(const vector<int> &a, const vector<int> &b, int cutoff) {
    int ln = 1;
    while (ln < (sz(a) + sz(b))) ln <<= 1;

    fore(i, 0, ln) aa[i] = (i < sz(a) ? a[i] : 0);
    fore(i, 0, ln) bb[i] = (i < sz(b) ? b[i] : 0);

    fft(aa, ln, false);
    fft(bb, ln, false);

    fore(i, 0, ln) cc[i] = mul(aa[i], bb[i]);
    fft(cc, ln, true);

    vector<int> ans(cc, cc + ln);
    while ((ans.size() > 0 && ans.back() == 0) || ans.size() > cutoff) ans.pop_back();
    return ans;
}

vector<int> fact, ifact;

void precalc_factorials(int s) {
    fact.resize(s);
    fact[0] = 1;
    for (int i = 1; i < s; i++) fact[i] = mul(fact[i - 1], i);
    ifact.resize(s);
    for (int i = 0; i < s; i++) ifact[i] = inv(fact[i]);
}

int choose(int n, int k) {
    if (n < 0 || n < k || k < 0) return 0;
    return mul(fact[n], mul(ifact[n - k], ifact[k]));
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> lens(k);
    for (int i = 0; i < k; i++) cin >> lens[i];

    int cutoff = n + 1;
    precalc_factorials(1 << LOGN);
    vector<int> segments(6);
    segments[0] = 1;
    for (auto len: lens)
        for (int i = 1; i <= len; i++) segments[i]++;
    vector<vector<int>> poly(6);
    for (int i = 0; i < 6; i++) {
        if (segments[i] == 0)
            poly[i] = {1};
        else {
            int choices = norm(m - i);
            int p = 1;
            poly[i].resize(cutoff);
            for (int k = 0; k < cutoff; k++) {
                int cur = mul(p, choose(k + segments[i] - 1, k));
                poly[i][k] = cur;
                p = mul(p, choices);
            }
        }
    }
    vector<int> ans = {1};
    for (int i = 0; i < 6; i++) ans = multiply(ans, poly[i], cutoff);

    int used = 0;
    int coef = 1;
    for (auto len: lens) {
        coef = mul(coef, fact[len]);
        used += len;
    }
    int res = mul(coef, ans[n - used]);
    cout << res << endl;
}

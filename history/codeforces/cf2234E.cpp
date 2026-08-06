#include <bits/stdc++.h>

using ll = long long;

using namespace std;

const long long MOD = 1e9 + 7;
const long long MAXN = 5e5 + 10;

vector<long long> fact(MAXN), ifact(MAXN);

long long qpow(long long a, long long b) {
    long long res = 1;
    while (b > 0) {
        if (b % 2 == 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b /= 2;
    }
    return res;
}

long long C(long long n, long long k) { return (fact[n] * ((ifact[k] * ifact[n - k]) % MOD)) % MOD; }

long long rec(long long l, long long r, vector<long long> &b) {
    if (r < l) {
        return 1;
    }
    if (l == r) {
        return b[l] == 1 ? 1 : 0;
    }
    for (long long d = 0; d <= (r - l + 1) / 2; d++) {
        long long i = d + l;
        if ((i - l + 1) * (r - i + 1) == b[i]) {
            return (((rec(l, i - 1, b) * rec(i + 1, r, b)) % MOD) * C(r - l, i - l)) % MOD;
        }
        i = r - d;
        if ((i - l + 1) * (r - i + 1) == b[i]) {
            return (((rec(l, i - 1, b) * rec(i + 1, r, b)) % MOD) * C(r - l, i - l)) % MOD;
        }
    }
    return 0;
}

void solve_linear() {
    long long n;
    cin >> n;
    vector<long long> b(n + 2);
    for (long long i = 1; i <= n; i++) cin >> b[i];

    long long s = accumulate(b.begin(), b.end(), 0ll);
    if (s != n * (n + 1) / 2) {
        cout << "0\n";
        return;
    }

    vector<long long> l(n + 2), r(n + 2);
    vector<int> in_q(n + 2, false);
    for (int i = 0; i <= n + 1; i++) {
        l[i] = i - 1, r[i] = i + 1;
    }
    queue<long long> q;
    auto f = [&](long long i) -> bool {
        if (i < 1 || i > n) return false;
        return (r[i] - i) * (i - l[i]) == b[i];
    };
    for (int i = 1; i <= n; i++) {
        if (f(i)) {
            q.push(i);
            in_q[i] = true; // 标记入队
        }
    }
    long long res = 1;
    int work = 0;
    while (!q.empty()) {
        auto i = q.front();
        q.pop();
        if (!f(i)) {
            return void(cout << "0\n");
        }
        res = res * C(r[i] - l[i] - 2, i - l[i] - 1) % MOD;
        work++;
        l[r[i]] = l[i];
        r[l[i]] = r[i];
        if (f(r[i])) {
            q.push(r[i]);
            in_q[r[i]] = true; // 标记入队
        }
        if (f(l[i])) {
            q.push(l[i]);
            in_q[l[i]] = true; // 标记入队
        }
    }

    if (work < n) return void(cout << "0\n");

    cout << res << '\n';
}

void solve() {
    long long n;
    cin >> n;
    vector<long long> b(n);
    for (long long i = 0; i < n; i++) cin >> b[i];

    long long s = accumulate(b.begin(), b.end(), 0ll);
    if (s != n * (n + 1) / 2) {
        cout << "0\n";
        return;
    }

    cout << rec(0, n - 1, b) << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    fact[0] = 1;
    for (long long i = 1; i < MAXN; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    ifact[MAXN - 1] = qpow(fact[MAXN - 1], MOD - 2);
    for (long long i = MAXN - 2; i >= 0; i--) {
        ifact[i] = ifact[i + 1] * (i + 1) % MOD;
    }

    long long t;
    cin >> t;
    while (t--) {
        solve();
    }
}

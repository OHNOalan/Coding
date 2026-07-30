#include <bits/stdc++.h>

using namespace std;

constexpr int MOD = 1'000'000'007;

inline int add(int a, int b) { return a + b >= MOD ? a + b - MOD : a + b; }
inline int sub(int a, int b) { return a - b < 0 ? a - b + MOD : a - b; }
inline int mul(int a, int b) { return int(1LL * a * b % MOD); }
inline void inc(int &a, int b) { a = add(a, b); }

int p;

int calc_w(int d, int rm, int nrm) {
    if (rm) return sub(p, calc_w(d, 0, nrm));
    return d + 1 - nrm; // == 0 when d == 0 && nrm == 1
}

int calc_W(int c, int rm, int nrm) {
    if (rm) return sub(mul(p, c + 1), calc_W(c, 0, nrm));
    if (c & 1) return sub(mul(c + 2, (c + 1) >> 1), nrm * (c + 1));
    return sub(mul(c + 1, (c + 2) >> 1), nrm * (c + 1));
}

vector<int> to_base_p(const string &dec) {
    vector<int> cur(dec.size());
    for (size_t i = 0; i < dec.size(); i++) cur[i] = dec[i] - '0';

    vector<int> digits;
    while (!cur.empty()) {
        vector<int> next;
        long long rem = 0;
        for (int d: cur) {
            rem = rem * 10 + d;
            if (!next.empty() || rem >= p) next.push_back(int(rem / p));
            rem %= p;
        }
        digits.push_back(int(rem));
        cur = std::move(next);
    }
    reverse(digits.begin(), digits.end());
    return digits;
}

int alpha, n;
vector<int> A;

void solve() {
    string s;
    cin >> p >> alpha >> s;

    A = to_base_p(s);
    n = static_cast<int>(A.size());
    if (alpha > n) {
        cout << 0 << "\n";
        return;
    }

    vector<vector<array<array<int, 2>, 2>>> memo;
    memo.assign(n, vector<array<array<int, 2>, 2>>(alpha + 1));
    for (auto &row: memo)
        for (auto &cell: row)
            for (auto &side: cell) side.fill(-1);

    cout << [&](this auto self, int i, int pw, int eq, int rm) -> int {
        if (i == n) return (pw == alpha && rm == 0) ? 1 : 0;

        int &res = memo[i][pw][eq][rm];
        if (res != -1) return res;

        res = 0;
        int d = A[i];
        for (int nrm = 0; nrm <= 1; nrm++) {
            int npw = min(pw + nrm, alpha);
            if (!eq) {
                inc(res, mul(calc_W(p - 1, rm, nrm), self(i + 1, npw, 0, nrm)));
            } else {
                if (d != 0) inc(res, mul(calc_W(d - 1, rm, nrm), self(i + 1, npw, 0, nrm)));
                inc(res, mul(calc_w(d, rm, nrm), self(i + 1, npw, 1, nrm)));
            }
        }
        return res;
    }(0, 0, 1, 0) << "\n";

    // vector dp(n + 1, vector(alpha + 1, array<array<int, 2>, 2>{}));
    // dp[0][0][1][0] = 1;
    // for (int i = 0; i < n; i++) {
    //     int d = A[i];
    //     for (int pw = 0; pw <= alpha; pw++) {
    //         for (int eq = 0; eq < 2; eq++) {
    //             for (int rm = 0; rm < 2; rm++) {
    //                 int cur = dp[i][pw][eq][rm];
    //                 if (cur == 0) continue;
    //                 for (int nrm = 0; nrm < 2; nrm++) {
    //                     int npw = min(pw + nrm, alpha);
    //                     if (!eq) {
    //                         inc(dp[i + 1][npw][0][nrm], mul(cur, calc_W(p - 1, rm, nrm)));
    //                     } else {
    //                         if (d != 0) {
    //                             inc(dp[i + 1][npw][0][nrm], mul(cur, calc_W(d - 1, rm, nrm)));
    //                         }
    //                         inc(dp[i + 1][npw][1][nrm], mul(cur, calc_w(d, rm, nrm)));
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    // int ans = 0;
    // for (int eq = 0; eq < 2; eq++) inc(ans, dp[n][alpha][eq][0]);
    // cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    solve();
}

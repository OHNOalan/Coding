#include <bits/stdc++.h>

using namespace std;

const int MOD = 1e9 + 7;

long long qpow(long long a, long long b) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

void solve() {
    long long N;
    cin >> N;
    string s = format("{:b}", N);
    int n = s.size();

    static long long dp[65][5][2][2][2];
    memset(dp, -1, sizeof(dp));

    auto dfs = [&](this auto &&self, int i, int d, int limA, int limB, int limC) -> long long {
        if (d < -2 || d > 2) return 0;
        if (i == n) return d == 0;

        if (dp[i][d + 2][limA][limB][limC] != -1) return dp[i][d + 2][limA][limB][limC];

        long long res = 0;
        int maxA = limA ? (s[i] - '0') : 1;
        int maxB = limB ? (s[i] - '0') : 1;
        int maxC = limC ? (s[i] - '0') : 1;

        static const int choices[4][3] = {{0, 0, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 0}};

        for (auto &choice: choices) {
            int a = choice[0], b = choice[1], c = choice[2];
            if (a <= maxA && b <= maxB && c <= maxC) {
                int nd = d * 2 + (a + c - 2 * b);
                int nlimA = limA && (a == maxA);
                int nlimB = limB && (b == maxB);
                int nlimC = limC && (c == maxC);

                res = (res + self(i + 1, nd, nlimA, nlimB, nlimC)) % MOD;
            }
        }

        return dp[i][d + 2][limA][limB][limC] = res;
    };
    long long ans = dfs(0, 0, 1, 1, 1);
    ans = (ans - 1 + MOD) % MOD;
    ans = ans * qpow(2, MOD - 2) % MOD;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
    return 0;
}

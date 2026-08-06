#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int N = 1e5 + 1;
const int MOD = 998244353;
i64 F[N], INVF[N];

i64 qmul(i64 a, i64 b) {
    i64 res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

void init() {
    F[0] = 1;
    for (int i = 1; i < N; i++) {
        F[i] = F[i - 1] * i % MOD;
    }
    INVF[N - 1] = qmul(F[N - 1], MOD - 2);
    for (int i = N - 1; i > 0; i--) {
        INVF[i - 1] = INVF[i] * i % MOD;
    }
    assert(INVF[0] == 1);
    assert(INVF[1] == 1);
}

i64 C(int n, int r) {
    if (r < 0 || r > n) return 0;
    return F[n] * INVF[r] % MOD * INVF[n - r] % MOD;
}

void solve() {
    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;

    i64 ans = 0;

    vector pre(3, vector<int>(n + 1));
    for (int x = 0; x < 3; x++) {
        for (int i = 0; i < n; i++) {
            pre[x][i + 1] = pre[x][i] + (s[i] == "01?"[x]);
        }
    }

    for (int c = 0; c < 2; c++) {
        if (pre[!c][n - k + 1]) {
            continue;
        }

        int q = pre[2][n] - pre[2][n - k + 1];
        int cnt = pre[c][n] - pre[c][n - k + 1];

        for (int i = 0; i <= q; i++) {
            if (cnt + i >= k / 2) {
                ans += C(q, i);
                ans %= MOD;
            }
        }
    }

    int cnt[3]{};
    cnt[2] = k - 1;

    vector<array<int, 2>> a(k - 1);

    for (int i = n - 1; i >= 0; i--) {
        if (i + k < n) {
            for (int c = 0; c < 2; c++) {
                if (pre[!c][i + 1]) {
                    continue;
                }
                int j = (i + 1) % (k - 1);
                if (a[j][c]) {
                    continue;
                }
                if (a[j][!c]) {
                    ans += C(cnt[2], k / 2 - cnt[c]);
                    ans %= MOD;
                } else {
                    ans += C(cnt[2] - 1, k / 2 - cnt[c]);
                    ans %= MOD;
                }
            }
        }

        int j = i % (k - 1);
        if (s[i] != '?') {
            a[j][s[i] - '0']++;
            if (a[j][0] && a[j][1]) {
                break;
            }
            if (a[j][0] + a[j][1] == 1) {
                cnt[2]--;
                cnt[s[i] - '0']++;
            }
        }
    }

    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();

    int t;
    cin >> t;
    while (t--) solve();
}

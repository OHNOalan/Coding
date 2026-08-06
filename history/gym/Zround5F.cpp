#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    string s, t;
    cin >> s >> t;
    int n = s.size();
    int m = t.size();
    vector<vector<int> > nxt(26, vector<int>(n+2, n));
    vector<vector<int> > pre(m + 1, vector<int>(m + 1, n + 1));
    vector<vector<int> > suf(m + 1, vector<int>(m + 1, n + 1));

    auto gen_nxt = [&]() {
        for (int j = 0; j < 26; ++j) {
            char c = 'a' + j;
            int r = n;
            for (int i = n - 1; i >= 0; --i) {
                if (s[i] == c) r = i;
                nxt[j][i] = r;
            }
        }
    };

    auto dp = [&](vector<vector<int> > &d) {
        d[0][0] = 0;
        for (int i = 0; i < m; i++) {
            d[i + 1][0] = 0;
            for (int j = 0; j <= i; j++) {
                d[i + 1][j + 1] = min(nxt[t[i] - 'a'][d[i][j]] + 1, d[i][j + 1]);
            }
        }
    };

    gen_nxt();
    dp(pre);
    ranges::reverse(s);
    ranges::reverse(t);
    gen_nxt();
    dp(suf);
    for (int i = 0, j = m - 1; i < m; i++, j--) {
        int ans = 0;
        for (int l = 0, r = j; l <= i && pre[i][l] <= n; ++l) {
            while (pre[i][l] + suf[j][r] > n) r--;
            ans = max(ans, l + r);
        }
        cout << ans << " \n"[i == m - 1];
    }
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

constexpr i64 M = 1e9 + 9;
constexpr i64 B = 641;
constexpr int N = 1e5 + 1;

vector<i64> pw(N);

void init() {
    pw[0] = 1;
    for (int i = 1; i < N; i++) pw[i] = pw[i - 1] * B % M;
}

constexpr int D = 300;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();
    int n;
    cin >> n;

    vector<string> s(n);
    for (int i = 0; i < n; i++) cin >> s[i];

    vector<vector<i64>> hsf(n), hsr(n);
    unordered_map<i64, i64> cntf, cntr, cnt;

    for (int i = 0; i < n; i++) {
        hsf[i].resize(s[i].size() + 1);
        for (int j = 0; j < s[i].size(); j++) hsf[i][j + 1] = (hsf[i][j] * B + s[i][j]) % M;
        hsr[i].resize(s[i].size() + 1);
        for (int j = 0; j < s[i].size(); j++) hsr[i][j + 1] = (hsr[i][j] * B + s[i][s[i].size() - j - 1]) % M;
        cntf[hsf[i].back()]++;
        cntr[hsr[i].back()]++;
    }

    auto get = [&](int i, int l, int r) { return (hsf[i][r] - hsf[i][l] * pw[r - l] % M + M) % M; };

    i64 rt = 0;
    for (int i = 0; i < n; i++) rt += cntr[hsf[i].back()];

    for (int i = 0; i < n; i++)
        if (s[i].size() >= D)
            for (int j = 0; j < n; j++) {
                i64 h = (hsf[i].back() * pw[s[j].size()] % M + hsf[j].back()) % M;
                cnt[h]++;
            }

    for (int i = 0; i < n; i++)
        if (s[i].size() >= D)
            for (int j = 0; j < n; j++)
                if (s[j].size() < D) {
                    i64 h = (hsf[j].back() * pw[s[i].size()] % M + hsf[i].back()) % M;
                    cnt[h]++;
                }

    i64 ans = 0;

    auto qry = [&](string &t, int ls, int rs) {
        int m = rs - ls;
        vector<i64> hs(m + 1);
        for (int i = 0; i < m; i++) hs[i + 1] = (hs[i] * B + t[ls + i]) % M;
        auto gt = [&](int l, int r) { return (hs[r] - hs[l] * pw[r - l] % M + M) % M; };
        i64 res = cnt[gt(0, m)];
        if (rs - ls < 610) {
            for (int i = 0; i < rs - ls - 1; i++) {
                if (i + 1 < D && m - i - 1 < D)
                    res += cntf[gt(0, i + 1)] * cntf[gt(i + 1, m)];
            }
        }
        return res;
    };

    unordered_map<i64, i64> cntpf, cntpr;
    for (int i = 0; i < n; i++)
        if (s[i].size() >= D) {
            for (int j = 0; j < n; j++)
                if (abs(int(s[i].size()) - int(s[j].size())) >= D) {
                    if (s[j].size() > s[i].size()) {
                        int f = 1;
                        for (int k = 0; k < s[i].size(); k++) {
                            if (s[i][k] != s[j][s[j].size() - k - 1]) {
                                f = 0;
                                break;
                            }
                        }
                        if (f) {
                            cntpf[get(j, 0, s[j].size() - s[i].size())]++;
                        }
                    } else {
                        int f = 1;
                        for (int k = 0; k < s[j].size(); k++) {
                            if (s[i][k] != s[j][s[j].size() - k - 1]) {
                                f = 0;
                                break;
                            }
                        }
                        if (f) {
                            cntpr[get(i, s[j].size(), s[i].size())]++;
                        }
                    }
                }
        }

    for (auto &s: s) {
        int m = s.size();
        vector<int> d(m);
        for (int i = 0, l = 0, r = -1; i < m; i++) {
            int k = (i > r) ? 1 : min(d[l + r - i], r - i + 1);
            while (0 <= i - k && i + k < m && s[i - k] == s[i + k]) k++;
            d[i] = k--;
            if (i + k > r) {
                l = i - k;
                r = i + k;
            }
        }
        for (int i = 0; i < m; i++) {
            if (d[i] * 2 - 1 == m) {
                ans += rt;
            } else if (d[i] == i + 1) {
                ans += qry(s, i + d[i], m);
                if (m - 2 * d[i] + 1 >= D)
                    ans += cntpf[get(i, i + d[i], m)];
                else {}
            } else if (d[i] == m - i) {
                ans += qry(s, 0, i - d[i]);
                if (m - 2 * d[i] + 1 >= D)
                    ans += cntpr[get(i, 0, i - d[i])];
                else {}
            }
        }
    }
    cout << ans;
}

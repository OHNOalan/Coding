#include <bits/stdc++.h>

using namespace std;

const int A = 1e6;
const int INF = INT_MAX / 2;

int I(int val, int l, int r) {
    if (val >= INF) return INF;
    return val >= l ? (val > r ? r : val) : 0;
}

void solve() {
    int n;
    cin >> n;
    vector<int> p(n), c(n);
    for (auto &x: p) cin >> x;
    for (auto &x: c) cin >> x;
    vector<int> mnp(A + 1, INF), mnc(A + 1, INF);
    int mn = INF;
    for (int i = 0; i < n; i++) {
        mnp[p[i]] = min(mnp[p[i]], c[i]);
        mnc[c[i]] = min(mnc[c[i]], p[i]);
        mn = min(mn, p[i] + c[i]);
    }
    vector<int> pmnp(A + 2, INF), pmnc(A + 2, INF);
    for (int i = 0; i <= A; i++) {
        pmnp[i + 1] = min(pmnp[i], mnp[i]);
        pmnc[i + 1] = min(pmnc[i], mnc[i]);
    }
    vector<int> smnp(A + 2, INF), smnc(A + 2, INF);
    for (int i = A; i >= 0; i--) {
        smnp[i] = min(smnp[i + 1], mnp[i]);
        smnc[i] = min(smnc[i + 1], mnc[i]);
    }
    int m;
    cin >> m;
    vector<int> tp(m), tc(m), d(m);
    for (auto &x: tp) cin >> x;
    for (auto &x: tc) cin >> x;
    for (auto &x: d) cin >> x;
    vector<int> ans(m);
    for (int i = 0; i < m; i++) {
        int tpl = tp[i];
        int tpr = tpl + d[i];
        int tcl = tc[i];
        int tcr = tcl + d[i];
        int itpr = min(tpr, A + 1);
        int itcr = min(tcr, A + 1);
        ans[i] = min({
                mn,
                I(pmnp[tpl], tcl, tcr),
                I(pmnc[tcl], tpl, tpr),
                I(smnp[itpr], tcl, tcr) + tpr,
                I(smnc[itcr], tpl, tpr) + tcr,
        });
    }
    for (auto &x: ans) cout << x << '\n';
}

// learn: simplify the case, and relax certain constraints with cover of others to simplify the computation

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
#ifdef LOCAL
    cin >> t;
#endif
    while (t--) solve();
}

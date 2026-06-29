#include <bits/stdc++.h>

using namespace std;

struct nd {
    int l = 0, r = 0, s = 0, a0 = 0;
};

vector<nd> tr;
int B;

void pull(int p) {
    auto &[l, r, s, a0] = tr[p];
    nd &L = tr[p << 1], &R = tr[p << 1 | 1];
    l = L.a0 ? L.s + R.l : L.l;
    r = R.a0 ? R.s + L.r : R.r;
    s = max({l, r, L.r + R.l, L.s, R.s});
    a0 = L.a0 & R.a0;
}

vector<int> lose;
int d;

void Mdf(int p, int v) {
    p += B;
    tr[p] = v ? nd{} : nd{1, 1, 1, 1};
    if (v == 2)
        lose.push_back(p - B);
    else
        for (p >>= 1; p; p >>= 1) pull(p);
}

void Dfs(int p, int rg) {
    if (tr[p].s <= d && tr[p].r + rg <= d) return;
    if (p >= B) {
        if (tr[p].s) Mdf(p - B, 2);
        return;
    }
    Dfs(p << 1, tr[p << 1 | 1].l + tr[p << 1 | 1].a0 * rg);
    Dfs(p << 1 | 1, rg);
}

void solve() {
    int res = 1;
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (B = 1; B <= n; B <<= 1);
    tr.assign(B << 1, nd{});

    vector<vector<int>> dR(n + 1);
    for (int h = n; h; h--) dR[n + 1 - h] = {h};
    for (int i = n; i; i--) {
        lose = {};
        d = a[i];
        Dfs(1, 0);
        for (int h: lose) {
            res += i == 1 && h == 1;
            Mdf(h, 1);
            if (i - h > 1) dR[i - h].push_back(h);
        }
        for (int h: dR[i]) Mdf(h, 0);
    }
    cout << res << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

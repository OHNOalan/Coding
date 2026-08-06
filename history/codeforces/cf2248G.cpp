#include <bits/stdc++.h>

using namespace std;

constexpr int MX = 125'005;

int Lgcd(const vector<int> &v) {
    return accumulate(v.begin(), v.end(), 0, [](int a, int b) { return gcd(a, b); });
}

void solve() {
    int n, m, s;
    cin >> n >> m >> s;
    vector<int> c(n);
    vector<pair<int, int>> ab(m);
    for (auto &x: c) cin >> x;
    for (auto &[a, b]: ab) cin >> a >> b;
    ab.insert(ab.begin(), make_pair(0, 0));
    m += 1;

    bitset<MX> p;
    p.set(0);
    for (int x: c) p.set(x);

    const int V = max(s, ab.back().first);
    for (int i = 1; i <= V; i++) {
        if (p[i]) p |= (p << i);
    }

    vector<int> l(V + 1, s + 1);
    l[0] = 0;
    int mnip = s + 1;
    vector<int> I, D;
    for (int i = 0, abp = 0; i <= V; i++) {
        if (p.test(i)) {
            while (abp + 1 < m && ab[abp + 1].first <= i) abp++;
            auto [a, b] = ab[abp];
            if (b <= i) {
                l[i - b] = min(l[i - b], i);
                D.push_back(i - b);
            } else {
                I.push_back(b - i);
                mnip = min(mnip, i);
            }
        }
    }

    vector<pair<int, int>> lv;
    for (int i = 1; i <= V; i++) {
        if (l[i] <= s) lv.emplace_back(l[i], i);
    }
    const int ln = lv.size();
    ranges::sort(lv);

    bitset<MX> f, L;
    f.set(0);
    for (int i = 1, lp = 0; i < mnip; ++i) {
        while (lp < ln && lv[lp].first <= i) {
            L.set(MX - lv[lp].second);
            lp++;
        }
        f[i] = (f & (L >> (MX - i))).any();
    }
    for (int i = 1; i < mnip; ++i) cout << (f[i] ? "YES" : "NO") << '\n';

    int Ig = Lgcd(I), Dg = Lgcd(D), cg = Lgcd(c);
    int ugcd = gcd(gcd(Ig, Dg), gcd(cg, ab.back().second));
    nth_element(c.begin(), c.begin(), c.end());
    for (int i = mnip; i <= s; ++i) cout << ((i % ugcd == 0 && ab[1].first > c.front()) ? "YES" : "NO") << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
#ifdef LOCAL
    cin >> t;
#endif
    while (t--) solve();
}

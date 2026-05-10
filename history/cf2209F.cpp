#include <bits/stdc++.h>

using namespace std;

struct FLONG {
    long long val;
    int nd;
    FLONG(long long v = 0, int d = 0) : val(v), nd(d) {}
    bool operator<(const FLONG &b) const { return val > b.val || (val == b.val && nd < b.nd); }
};
struct pir {
    int to, len;
    pir(int t = 0, int l = 0) : to(t), len(l) {}
    bool operator<(const pir &b) const { return len > b.len || (len == b.len && to < b.to); }
};

void solve() {
    int n, k;
    cin >> n >> k;
    long long mxa = 0, sum = 0;
    vector<long long> ans(n + 1);
    vector<int> a(n + 1);
    vector<long long> b(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        mxa = max(mxa, 1ll * a[i]);
    }

    vector<vector<int>> g(n + 1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    if (k == 1) {
        cout << mxa << '\n';
        return;
    }

    set<FLONG> s;
    set<FLONG>::iterator o;
    vector<pir> mn(n + 1), se(n + 1), lf(n + 1);
    vector<int> tt(n + 1);

    lf[0].len = ~0x3f3f3f3f;
    auto srh = [&](this auto &&srh, int u, int fa) -> void {
        mn[u] = pir(u, 0);
        se[u] = pir(0, ~0x3f3f3f3f);
        lf[u] = pir(0, ~0x3f3f3f3f);
        for (int v: g[u]) {
            if (v != fa) {
                srh(v, u);
                pir th = mn[v];
                th.len++;
                if (th < se[u]) swap(th, se[u]);
                if (se[u] < mn[u]) swap(se[u], mn[u]);
            }
        }
    };
    auto gt1 = [&]() -> void {
        sum = ans[1] = 0;
        ans[1] += a[1];
        for (int i = 2; i <= n; i++) {
            b[mn[i].to] += a[i];
            tt[i] = mn[i].to;
        }
        s.clear();
        for (int i = 2; i <= n; i++) {
            s.insert(FLONG(b[i], i));
        }
        o = s.begin();
        ans[1] += o->val;
        sum += o->val;
        for (int i = 1; i < k - 1; i++) {
            o++;
            ans[1] += o->val;
            sum += o->val;
        }
        s.insert(FLONG(-1ll, 0));
        s.insert(FLONG(-2ll, 0));
    };
    auto era = [&](int x) -> void {
        FLONG u = FLONG(b[x], x);
        if (!(*o < u)) {
            o++;
            sum += o->val;
            sum -= b[x];
        }
        s.erase(u);
    };
    auto ins = [&](int x) -> void {
        FLONG u = FLONG(b[x], x);
        s.insert(u);
        if (!(*o < u)) {
            sum -= o->val;
            sum += b[x];
            o--;
        }
    };
    auto stp = [&](int x) -> void {
        era(tt[x]);
        b[tt[x]] -= a[x];
        ins(tt[x]);
        tt[x] = 0;
    };
    auto sett = [&](int x, int ttp) -> void {
        tt[x] = ttp;
        era(tt[x]);
        b[tt[x]] += a[x];
        ins(tt[x]);
    };

    auto srho = [&](this auto &&srho, int u, int fa) -> void {
        for (int v: g[u]) {
            if (v == fa) continue;
            pir up = lf[fa];
            up.len++;
            lf[u] = min(up, mn[u].to == mn[v].to ? se[u] : mn[u]);

            stp(v);
            era(v);
            ins(u);
            sett(u, lf[u].to);
            ans[v] = sum + a[v];
            srho(v, u);
            ins(v);
            sett(v, mn[v].to);
            stp(u);
            era(u);
        }
    };
    srh(1, 0);
    gt1();
    srho(1, 0);
    for (int i = 1; i <= n; i++) {
        mxa = max(mxa, ans[i]);
    }
    cout << mxa << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

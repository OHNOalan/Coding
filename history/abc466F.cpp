#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MAX_NODES = 2E7;

struct Node {
    int ls, rs;
    ll sz;
    ll val;
    ll add;
};

vector<Node> tr(MAX_NODES);

int tot = 0;
int root;
ll curWidth;
uint32_t seed = 1337;
uint32_t fast_rnd() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

int clone(int p) {
    int q = ++tot;
    tr[q] = tr[p];
    return q;
}

void pushup(int p) { tr[p].sz = tr[tr[p].ls].sz + tr[tr[p].rs].sz + 1; }

void apply_add(int p, ll delta) {
    if (!p || delta == 0) return;
    tr[p].val += delta;
    tr[p].add += delta;
}

void pushdown(int p) {
    if (!tr[p].add) return;
    if (tr[p].ls) {
        tr[p].ls = clone(tr[p].ls);
        apply_add(tr[p].ls, tr[p].add);
    }
    if (tr[p].rs) {
        tr[p].rs = clone(tr[p].rs);
        apply_add(tr[p].rs, tr[p].add);
    }
    tr[p].add = 0;
}

void split(int p, ll k, int &x, int &y) {
    if (!p) {
        x = y = 0;
        return;
    }
    if (k >= tr[p].sz) {
        x = p;
        y = 0;
        return;
    }
    if (k <= 0) {
        x = 0;
        y = p;
        return;
    }
    p = clone(p);
    pushdown(p);
    if (tr[tr[p].ls].sz + 1 <= k) {
        x = p;
        split(tr[p].rs, k - tr[tr[p].ls].sz - 1, tr[p].rs, y);
    } else {
        y = p;
        split(tr[p].ls, k, x, tr[p].ls);
    }
    pushup(p);
}

int merge(int x, int y) {
    if (!x || !y) return x | y;
    unsigned __int128 r = (unsigned __int128) fast_rnd() * (unsigned __int128) (tr[x].sz + tr[y].sz);
    int p;
    if ((ll) (r >> 32) < tr[x].sz) {
        p = clone(x);
        pushdown(p);
        tr[p].rs = merge(tr[p].rs, y);
    } else {
        p = clone(y);
        pushdown(p);
        tr[p].ls = merge(x, tr[p].ls);
    }
    pushup(p);
    return p;
}

int shift_node(int p, ll delta) {
    if (!p || delta == 0) return p;
    int q = clone(p);
    apply_add(q, delta);
    return q;
}

ll count_leq(int p, ll t) {
    ll inc = 0, cnt = 0;
    while (p) {
        ll vv = tr[p].val + inc;
        if (vv <= t) {
            cnt += tr[tr[p].ls].sz + 1;
            inc += tr[p].add;
            p = tr[p].rs;
        } else {
            inc += tr[p].add;
            p = tr[p].ls;
        }
    }
    return cnt;
}

int repeat_concat(int base, ll width, ll q) {
    if (q <= 0) return 0;
    int res = 0;
    ll ca = 0;
    int b = base;
    ll cb = width;
    while (q > 0) {
        if (q & 1) {
            res = merge(res, shift_node(b, ca));
            ca += cb;
        }
        q >>= 1;
        if (q > 0) {
            b = merge(b, shift_node(b, cb));
            cb *= 2;
        }
    }
    return res;
}

void grow(ll D2) {
    ll D = curWidth;
    ll q = D2 / D, r = D2 % D;

    int x = repeat_concat(root, D, q);
    ll cnt = count_leq(root, r - 1);
    int tx, ty;
    split(root, cnt, tx, ty);
    int y = shift_node(tx, q * D);
    root = merge(x, y);
    curWidth = D2;
}

void solve() {
    int n;
    ll X;
    cin >> n >> X;
    vector<ll> a(n);
    for (auto &v: a) cin >> v;

    root = ++tot;
    tr[root] = {0, 0, 1, 0, 0};
    curWidth = a[n - 1];

    for (int idx = n - 2; idx >= 0; --idx) grow(a[idx]);

    ll zeroInSet = count_leq(root, 0);
    grow(X + 1);
    cout << (tr[root].sz - zeroInSet) << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // #ifdef LOCAL
    cin >> t;
    // #endif
    while (t--) {
        tot = 0;
        solve();
    }
}

#include <bits/stdc++.h>

using namespace std;
using ll = long long;

const ll INF = 1e18;

struct Node {
    ll mx, mn;
    int lazy;
} tree[800005];

vector<ll> global_a;
vector<bool> global_need;

void push_up(int rt) {
    tree[rt].mx = max(tree[rt * 2].mx, tree[rt * 2 + 1].mx);
    tree[rt].mn = min(tree[rt * 2].mn, tree[rt * 2 + 1].mn);
}

void apply_flip(int rt) {
    tree[rt].lazy ^= 1;
    ll old_mx = tree[rt].mx;
    tree[rt].mx = (tree[rt].mn == INF) ? -INF : -tree[rt].mn;
    tree[rt].mn = (old_mx == -INF) ? INF : -old_mx;
}

void push_down(int rt) {
    if (tree[rt].lazy) {
        apply_flip(rt * 2);
        apply_flip(rt * 2 + 1);
        tree[rt].lazy = 0;
    }
}

void build(int rt, int l, int r) {
    tree[rt].lazy = 0;
    if (l == r) {
        if (global_need[l]) {
            tree[rt].mx = global_a[l];
            tree[rt].mn = global_a[l];
        } else {
            tree[rt].mx = -INF;
            tree[rt].mn = INF;
        }
        return;
    }
    int mid = (l + r) / 2;
    build(rt * 2, l, mid);
    build(rt * 2 + 1, mid + 1, r);
    push_up(rt);
}

void update_flip(int rt, int l, int r, int L, int R) {
    if (L <= l && r <= R) {
        apply_flip(rt);
        return;
    }
    push_down(rt);
    int mid = (l + r) / 2;
    if (L <= mid) update_flip(rt * 2, l, mid, L, R);
    if (R > mid) update_flip(rt * 2 + 1, mid + 1, r, L, R);
    push_up(rt);
}

void update_disable(int rt, int l, int r, int idx) {
    if (l == r) {
        tree[rt].mx = -INF;
        tree[rt].mn = INF;
        return;
    }
    push_down(rt);
    int mid = (l + r) / 2;
    if (idx <= mid)
        update_disable(rt * 2, l, mid, idx);
    else
        update_disable(rt * 2 + 1, mid + 1, r, idx);
    push_up(rt);
}

int find_positive_idx(int rt, int l, int r) {
    if (l == r) return l;
    push_down(rt);
    int mid = (l + r) / 2;
    if (tree[rt * 2].mx > 0)
        return find_positive_idx(rt * 2, l, mid);
    else
        return find_positive_idx(rt * 2 + 1, mid + 1, r);
}

void solve() {
    int n;
    cin >> n;
    vector<ll> a(n);
    for (auto &x: a) cin >> x;
#ifdef LOCAL
    int k;
    cin >> k;
    vector<int> op(k);
    for (auto &x: op) cin >> x, --x;
    auto b = a;
    for (int x: op) {
        assert(x < n);
        for (int i = 0; i <= x; ++i) {
            b[i] = -b[i];
        }
    }
    cout << k << '\n';
    for (auto x: op) cout << x + 1 << " ";
    cout << '\n';
    ll stdAns = accumulate(b.begin(), b.end(), 0LL);
#endif

    // [0, 1] : canflip mn mx, [2, 3] : cannot flip mn mx
    vector<array<ll, 4>> mnx(n + 1, {0, 0});
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (j & 1)
                mnx[i + 1][j] = max(a[i] + mnx[i][j], -mnx[i][1 ^ j] - a[i]);
            else
                mnx[i + 1][j] = min(a[i] + mnx[i][j], -mnx[i][1 ^ j] - a[i]);
        }
        for (int j = 2; j < 4; ++j) {
            mnx[i + 1][j] = a[i] + mnx[i][j];
            if (a[i] > 0) {
                if (j & 1)
                    mnx[i + 1][j] = max(mnx[i + 1][j], -mnx[i][3 ^ j] - a[i]);
                else
                    mnx[i + 1][j] = min(mnx[i + 1][j], -mnx[i][3 ^ j] - a[i]);
            }
        }
    }

    ll cur = mnx[n][3];
    int j = 3;
    vector<bool> need_op(n, false);
    int total_ops = 0;

    for (int i = n - 1; i >= 0; --i) {
        if (j < 2) {
            if (cur == -mnx[i][1 ^ j] - a[i]) {
                cur = mnx[i][1 ^ j];
                j = 1 ^ j;
                need_op[i] = true;
                total_ops++;
            } else {
                cur = mnx[i][j];
            }
        } else {
            if (a[i] > 0 && cur == -mnx[i][3 ^ j] - a[i]) {
                cur = mnx[i][3 ^ j];
                j = 3 ^ j;
                need_op[i] = true;
                total_ops++;
            } else {
                cur = mnx[i][j];
            }
        }
    }

    global_a = a;
    global_need = need_op;
    build(1, 0, n - 1);

    vector<int> final_ops;

    for (int step = 0; step < total_ops; ++step) {
        int idx = find_positive_idx(1, 0, n - 1);
        final_ops.push_back(idx + 1);
        update_disable(1, 0, n - 1, idx);
        update_flip(1, 0, n - 1, 0, idx);
    }

    cout << final_ops.size() << '\n';
    for (auto &x: final_ops) cout << x << ' ';
    cout << '\n';

#ifdef LOCAL
    auto final_b = a;
    for (int x: final_ops) {
        int idx = x - 1;
        assert(final_b[idx] > 0);
        for (int i = 0; i <= idx; ++i) {
            final_b[i] = -final_b[i];
        }
    }
    ll final_stdAns = accumulate(final_b.begin(), final_b.end(), 0LL);
    cout << mnx[n][3] << " " << final_stdAns << '\n';
    assert(final_stdAns == mnx[n][3]);
#endif
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

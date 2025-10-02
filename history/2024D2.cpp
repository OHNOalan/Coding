#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 4e5 + 10;

ll a[MAXN], b[MAXN];
ll prefSum[MAXN], dp[MAXN];

struct SegmentTree {
    vector<ll> tree;
    int size;

    void init(int n) {
        size = n;
        tree.assign(4 * n, 0);
    }

    void update(int o, int l, int r, int pos, ll val) {
        if (l == r) {
            tree[o] = val;
            return;
        }
        int m = (l + r) >> 1;
        if (pos <= m)
            update(2 * o, l, m, pos, val);
        else
            update(2 * o + 1, m + 1, r, pos, val);
        tree[o] = max(tree[2 * o], tree[2 * o + 1]);
    }

    ll query(int o, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr)
            return tree[o];
        int m = (l + r) / 2;
        ll res = 0;
        if(ql <= m) res = query(2 * o, l, m, ql, qr);
        if(qr > m) res = max(res, query(2 * o + 1, m+1, r, ql, qr));
        return res;
    }
} segTree;

void solve() {
    ll n;
    cin >> n;
    for (ll i = 0; i < n; i++) {
        cin >> a[i];
        prefSum[i + 1] = prefSum[i] + a[i];
    }

    for (ll i = 0; i < n; i++) {
        cin >> b[i];
        b[i]--;
    }

    segTree.init(n + 1);
    segTree.update(1, 0, n, n, prefSum[n]);
    dp[n] = 0;

    for (ll i = n - 1; i >= 0; i--) {
        ll maxM = 0;
        if (b[i] > i)
            maxM = segTree.query(1, 0, n, i + 1, b[i]);
        dp[i] = max(a[i], maxM - prefSum[i + 1]);
        segTree.update(1, 0, n, i, dp[i] + prefSum[i]);
    }
    cout << dp[0] << '\n';
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}
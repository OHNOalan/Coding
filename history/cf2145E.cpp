#include <bits/stdc++.h>

#define LF 2 * v + 1
#define RT 2 * v + 2

using namespace std;

using i64 = long long;

vector<int> add, mn;

void push(int v) {
    add[LF] += add[v];
    add[RT] += add[v];
    add[v] = 0;
}

int getmin(int v) { return mn[v] + add[v]; }

void upd(int v) { mn[v] = min(getmin(LF), getmin(RT)); }

void init(int v, int l, int r) {
    if (l + 1 == r) {
        mn[v] = -l;
        return;
    }
    int m = (l + r) / 2;
    init(LF, l, m);
    init(RT, m, r);
    upd(v);
}

void init(int n) {
    add.assign(4 * n, 0);
    mn.resize(4 * n);
    init(0, 0, n);
}

void addVal(int v, int l, int r, int L, int R, int val) {
    assert(l < r);
    if (L <= l && r <= R) {
        add[v] += val;
        return;
    }
    int m = (l + r) / 2;
    push(v);
    if (L < m)
        addVal(LF, l, m, L, R, val);
    if (m < R)
        addVal(RT, m, r, L, R, val);
    upd(v);
}

int firstNeg(int v, int l, int r) {
    assert(l < r);
    if (l + 1 == r) {
        assert(getmin(v) < 0);
        return l;
    }
    push(v);
    int m = (l + r) / 2;
    int ans = -1;
    if (getmin(LF) < 0)
        ans = firstNeg(LF, l, m);
    else
        ans = firstNeg(RT, m, r);
    upd(v);
    assert(ans != -1);
    return ans;
}

void solve() {
    int ac, dr;
    cin >> ac >> dr;
    int n;
    cin >> n;
    int N = n + 2;
    init(N);

    vector<int> a(n), d(n);
    for (int &x: a) cin >> x;
    for (int &x: d) cin >> x;

    auto calc = [&](int i) -> int { return min(n, max(a[i] - ac, 0) + max(d[i] - dr, 0)); };

    for (int i = 0; i < n; i++) {
        int p = calc(i);
        addVal(0, 0, N, p + 1, N, 1);
    }

    int m;
    cin >> m;
    while (m--) {
        int k, na, nd;
        cin >> k >> na >> nd;
        k--;
        int p = calc(k);
        addVal(0, 0, N, p + 1, N, -1);
        a[k] = na, d[k] = nd;
        p = calc(k);
        addVal(0, 0, N, p + 1, N, 1);
        cout << firstNeg(0, 0, N) - 1 << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    solve();
}

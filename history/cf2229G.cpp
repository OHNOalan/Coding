#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using u128 = unsigned __int128;
using i128 = __int128;

void solve() {
    int n, x;
    i64 k;
    cin >> n >> k >> x, --x;
    vector<i64> h(n), d(n - 1);
    for (auto &x: h) cin >> x;
    for (auto &x: d) cin >> x;
    vector<i64> t(n);
    for (int i = x; i < n - 1; ++i) {
        t[i + 1] = max(t[i] + 1, d[i]);
    }
    for (int i = x - 1; i >= 0; --i) {
        t[i] = max(t[i + 1] + 1, d[i]);
    }
    t[x] = 1;

    vector<int> nh(n), lh(n);
    vector<pair<i64, i64>> stk;
    stk.reserve(n);
    stk.emplace_back(n, 2e9);
    for (int i = n - 1; i >= 0; --i) {
        while (stk.back().second <= h[i]) stk.pop_back();
        nh[i] = stk.back().first;
        stk.emplace_back(i, h[i]);
    }
    stk.clear();
    stk.emplace_back(-1, 2e9);
    for (int i = 0; i < n; ++i) {
        while (stk.back().second <= h[i]) stk.pop_back();
        lh[i] = stk.back().first;
        stk.emplace_back(i, h[i]);
    }

    vector<i64> dp(n);
    vector<pair<i64, i64>> vals;
    for (int i = 0; i < n; ++i) vals.emplace_back(make_pair(t[i], i));
    ranges::sort(vals);
    i64 ans = 0;
    vector<i64> ps(n + 1);
    for (int i = 0; i < n; ++i) {
        ps[i + 1] = ps[i] + h[i];
    }
    for (auto xx: vals) {
        int i = xx.second;
        if (dp[i] < 0) continue;
        ans = max(ans, (k - t[i] + 1) * h[i] + dp[i]);
        for (int j: {nh[i], lh[i]}) {
            if (j < 0 || j >= n) continue;
            if (abs(j - i) > t[j] - t[i]) {
                continue;
            }
            dp[j] = max(dp[j], dp[i] + (t[j] - t[i] - abs(j - i) + 1) * h[i] + ps[max(i, j)] - ps[min(i, j) + 1]);
        }
    }
    cout << ans << '\n';
}

void solve2() {
    int n, k, x;
    cin >> n >> k >> x, --x;
    vector<long long> h(n), pre(n + 1);
    for (int i = 0; i < n; i++) cin >> h[i];
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + h[i];
    vector<int> d(n - 1), t(n);
    for (int i = 0; i < n - 1; i++) cin >> d[i];
    for (int i = x - 1; i >= 0; i--) t[i] = max(t[i + 1] + 1, d[i]);
    for (int i = x + 1; i < n; i++) t[i] = max(t[i - 1] + 1, d[i - 1]);
    vector<int> nh(n, -1), lh(n, -1);
    vector<int> stk;
    for (int i = n - 1; i >= 0; i--) {
        while (!stk.empty() && h[stk.back()] <= h[i]) stk.pop_back();
        if (!stk.empty()) nh[i] = stk.back();
        stk.push_back(i);
    }
    stk.clear();
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && h[stk.back()] <= h[i]) stk.pop_back();
        if (!stk.empty()) lh[i] = stk.back();
        stk.push_back(i);
    }
    vector<long long> dp(n, LLONG_MIN);
    dp[x] = 0;
    long long ans = 0;
    auto process = [&](int i) {
        if (dp[i] < LLONG_MIN / 2) return;
        if (t[i] <= k) ans = max(ans, dp[i] + (k - t[i]) * h[i]);
        for (int j: {nh[i], lh[i]}) {
            if (j == -1) continue;
            long long dist = abs(j - i);
            if (t[j] - t[i] < dist) continue;
            long long day = t[j] - t[i] - dist;
            long long path = pre[max(i, j) + 1] - pre[min(i, j)] - h[i];
            dp[j] = max(dp[j], dp[i] + day * h[i] + path);
        }
    };
    process(x);
    int l = x - 1, r = x + 1;
    while (l >= 0 || r < n) process(r == n || (l >= 0 && t[l] <= t[r]) ? l-- : r++);
    cout << ans << '\n';
}


struct Line {
    mutable long long k, m, p;
    bool operator<(const Line &o) const { return k < o.k; }
    bool operator<(long long x) const { return p < x; }
    long long eval(long long x) { return k * x + m; }
};

struct LineContainer : multiset<Line, less<>> {
    static const long long inf = LLONG_MAX;
    long long div(long long a, long long b) { return a / b - ((a ^ b) < 0 && a % b); }
    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        if (x->k == y->k)
            x->p = x->m > y->m ? inf : -inf;
        else
            x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(long long k, long long m) {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }
    long long query(long long x) {
        assert(!empty());
        auto l = *lower_bound(x);
        return l.eval(x);
    }
};

void solve3() {
    int n, k, x;
    cin >> n >> k >> x, x--;
    vector<long long> h(n), pre(n + 1, 0);
    for (int i = 0; i < n; i++) cin >> h[i];
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + h[i];
    vector<long long> d(n - 1), t(n, 0);
    for (int i = 0; i < n - 1; i++) cin >> d[i];
    for (int i = x - 1; i >= 0; i--) t[i] = max(t[i + 1] + 1LL, d[i]);
    for (int i = x + 1; i < n; i++) t[i] = max(t[i - 1] + 1LL, d[i - 1]);
    LineContainer cht_L, cht_R;
    using pii = pair<long long, int>;
    priority_queue<pii, vector<pii>, greater<pii>> pq_L, pq_R;
    vector<long long> dp(n, -2e18);
    dp[x] = 0;
    auto add_state = [&](int j) {
        pq_R.push({t[j] - j, j});
        pq_L.push({t[j] + j, j});
    };
    add_state(x);
    long long ans = 0;
    if (t[x] <= k) ans = max(ans, dp[x] + (k - t[x]) * h[x]);
    int l = x - 1, r = x + 1;
    while (l >= 0 || r < n) {
        if (r == n || (l >= 0 && t[l] <= t[r])) {
            int i = l;
            long long X = t[i] + i;
            while (!pq_L.empty() && pq_L.top().first <= X) {
                int j = pq_L.top().second;
                pq_L.pop();
                cht_L.add(h[j], dp[j] - h[j] * (t[j] + j) + pre[j]);
            }
            dp[i] = cht_L.query(X) - pre[i];
            add_state(i);
            if (t[i] <= k) ans = max(ans, dp[i] + (k - t[i]) * h[i]);
            l--;
        } else {
            int i = r;
            long long X = t[i] - i;
            while (!pq_R.empty() && pq_R.top().first <= X) {
                int j = pq_R.top().second;
                pq_R.pop();
                cht_R.add(h[j], dp[j] - h[j] * (t[j] - j) - pre[j + 1]);
            }
            dp[i] = cht_R.query(X) + pre[i + 1];
            add_state(i);
            if (t[i] <= k) ans = max(ans, dp[i] + (k - t[i]) * h[i]);
            r++;
        }
    }

    cout << ans << '\n';
}

const i64 INF = 2e18;
// example cf2229G solve4()
struct LineLCT {
    i64 k, b;
    bool has_val;

    LineLCT() : k(0), b(0), has_val(false) {}
    LineLCT(i64 _k, i64 _b) : k(_k), b(_b), has_val(true) {}

    i64 eval(i64 x) const {
        if (!has_val) return -INF;
        return k * x + b;
    }
};

struct LiChaoTree {
    int n;
    vector<LineLCT> tree;
    vector<i64> xs;

    LiChaoTree(const vector<i64> &_xs) {
        xs = _xs;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        n = xs.size();
        tree.assign(n * 4 + 4, LineLCT());
    }

    void insert(int p, int l, int r, LineLCT nw) {
        if (!tree[p].has_val) {
            tree[p] = nw;
            return;
        }

        // 叶子节点直接比较
        if (l == r) {
            if (nw.eval(xs[l]) > tree[p].eval(xs[l])) {
                tree[p] = nw;
            }
            return;
        }

        int mid = l + (r - l) / 2;
        // 注意：评估直线时，代入的是真实的横坐标 xs[mid]
        bool left_better = (nw.eval(xs[l]) > tree[p].eval(xs[l]));
        bool mid_better = (nw.eval(xs[mid]) > tree[p].eval(xs[mid]));

        if (mid_better) {
            swap(tree[p], nw);
        }

        if (left_better != mid_better) {
            insert(p << 1, l, mid, nw);
        } else {
            insert(p << 1 | 1, mid + 1, r, nw);
        }
    }

    void add_line(i64 k, i64 b) {
        if (n == 0) return;
        insert(1, 0, n - 1, LineLCT(k, b));
    }

    i64 query(int p, int l, int r, int idx) {
        if (!tree[p].has_val) return -INF;

        i64 res = tree[p].eval(xs[idx]);
        if (l == r) return res;

        int mid = l + (r - l) / 2;
        if (idx <= mid)
            res = max(res, query(p << 1, l, mid, idx));
        else
            res = max(res, query(p << 1 | 1, mid + 1, r, idx));

        return res;
    }

    i64 get_max(i64 x) {
        if (n == 0) return -INF;
        // 找到真实坐标对应的离散化索引
        int idx = lower_bound(xs.begin(), xs.end(), x) - xs.begin();
        return query(1, 0, n - 1, idx);
    }
};

void solv4() {
    int n, k, x;
    cin >> n >> k >> x, x--;
    vector<i64> h(n), pre(n + 1, 0);
    for (int i = 0; i < n; i++) cin >> h[i];
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + h[i];
    vector<i64> d(n - 1), t(n, 0);
    for (int i = 0; i < n - 1; i++) cin >> d[i];
    for (int i = x - 1; i >= 0; i--) t[i] = max(t[i + 1] + 1LL, d[i]);
    for (int i = x + 1; i < n; i++) t[i] = max(t[i - 1] + 1LL, d[i - 1]);
    vector<i64> xs_L, xs_R;
    for (int i = 0; i <= x; i++) xs_L.push_back(t[i] + i);
    for (int i = x; i < n; i++) xs_R.push_back(t[i] - i);
    LiChaoTree lct_L(xs_L);
    LiChaoTree lct_R(xs_R);
    using pii = pair<i64, int>;
    priority_queue<pii, vector<pii>, greater<pii>> pq_L, pq_R;
    vector<i64> dp(n, -INF);
    dp[x] = 0;
    auto add_state = [&](int j) {
        pq_L.push({t[j] + j, j});
        pq_R.push({t[j] - j, j});
    };
    add_state(x);
    i64 ans = 0;
    if (t[x] <= k) ans = max(ans, dp[x] + (k - t[x]) * h[x]);
    int l = x - 1, r = x + 1;
    while (l >= 0 || r < n) {
        if (r == n || (l >= 0 && t[l] <= t[r])) {
            int i = l;
            i64 X = t[i] + i;
            while (!pq_L.empty() && pq_L.top().first <= X) {
                int j = pq_L.top().second;
                pq_L.pop();
                lct_L.add_line(h[j], dp[j] - h[j] * (t[j] + j) + pre[j]);
            }
            dp[i] = lct_L.get_max(X) - pre[i];
            add_state(i);
            if (t[i] <= k) ans = max(ans, dp[i] + (k - t[i]) * h[i]);
            l--;
        } else {
            int i = r;
            i64 X = t[i] - i;
            while (!pq_R.empty() && pq_R.top().first <= X) {
                int j = pq_R.top().second;
                pq_R.pop();
                lct_R.add_line(h[j], dp[j] - h[j] * (t[j] - j) - pre[j + 1]);
            }
            dp[i] = lct_R.get_max(X) + pre[i + 1];
            add_state(i);
            if (t[i] <= k) ans = max(ans, dp[i] + (k - t[i]) * h[i]);
            r++;
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

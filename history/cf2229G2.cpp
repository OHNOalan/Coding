#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

struct Line {
    mutable i64 k, m, p;
    bool operator<(const Line &o) const { return k < o.k; }
    bool operator<(i64 x) const { return p < x; }
    i64 eval(i64 x) { return k * x + m; }
};

struct LineContainer : multiset<Line, less<>> {
    static const i64 inf = LLONG_MAX;
    i64 div(i64 a, i64 b) { return a / b - ((a ^ b) < 0 && a % b); }
    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        if (x->k == y->k)
            x->p = x->m > y->m ? inf : -inf;
        else
            x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(i64 k, i64 m) {
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }
    i64 query(i64 x) {
        assert(!empty());
        auto l = *lower_bound(x);
        return l.eval(x);
    }
};

struct SideInfo {
    int pos;
    int best_house;
    Line cur_line;
    LineContainer cht;
    queue<pair<int, pair<i64, i64>>> delay_q;
};

void solve() {
    int n, t, x;
    cin >> n >> t >> x, --x;

    vector<i64> h(n), d(n - 1), ps(n + 1);
    for (auto &v: h) cin >> v;
    for (auto &v: d) cin >> v;
    for (int i = 0; i < n; ++i) ps[i + 1] = ps[i] + h[i];

    auto range_sum = [&](int a, int b) -> i64 {
        if (a > b || a < 0 || b >= n) return 0;
        return ps[b + 1] - ps[a];
    };

    for (int i = x - 2; i >= 0; --i) d[i] = max(d[i], d[i + 1] + 1);
    for (int i = x; i < n - 2; i++) d[i + 1] = max(d[i + 1], d[i] + 1);

    i64 ans = h[x] * t;

    vector<SideInfo> side(2);
    side[0] = {x, x, {h[x], 0, 0}, {}, {}};
    side[1] = {x, x, {h[x], 0, 0}, {}, {}};
    side[0].cht.add(h[x], 0);
    side[1].cht.add(h[x], 0);

    auto advance = [&](int dir) {
        int opp = dir ^ 1;
        side[dir].pos += (dir == 0) ? -1 : 1;
        int u = side[dir].pos;
        int open_time = (dir == 0) ? d[u] : d[u - 1];

        if (h[u] > side[dir].cur_line.k) {
            int pref_dist = abs(side[dir].best_house - u);
            int start_dist = abs(x - u);

            auto &oq = side[opp].delay_q;
            auto &ocht = side[opp].cht;
            while (!oq.empty() && oq.front().first <= open_time - start_dist) {
                ocht.add(oq.front().second.first, oq.front().second.second);
                oq.pop();
            }

            int same_base = side[dir].best_house;
            i64 cost_same = range_sum(min(u, same_base) + 1, max(u, same_base) - 1);

            i64 cost_opp = range_sum(min(u, x) + 1, max(u, x) - 1);

            i64 best = max(side[dir].cur_line.eval(open_time - pref_dist) + cost_same,
                           ocht.query(open_time - start_dist) + cost_opp);

            ans = max(ans, best + (t - open_time + 1) * h[u]);

            side[dir].best_house = u;
            side[dir].cur_line = {h[u], best - (open_time - 1) * h[u], 0};

            // 3. 投递给对侧的过路费：必须包含起点 x 自身的收益，但不包含 u 的收益
            int L_edge = (u < x) ? u + 1 : x;
            int R_edge = (u < x) ? x : u - 1;
            i64 cost_to_start = range_sum(L_edge, R_edge);

            i64 intercept = best - (open_time - 1 + start_dist) * h[u] + cost_to_start;
            side[dir].delay_q.push({open_time + start_dist, {h[u], intercept}});
        }
    };

    while (side[0].pos > 0 || side[1].pos < n - 1) {
        int left_time = (side[0].pos == 0) ? 2e9 : d[side[0].pos - 1];
        int right_time = (side[1].pos == n - 1) ? 2e9 : d[side[1].pos];

        if (min(left_time, right_time) > t) break;

        if (left_time <= right_time)
            advance(0);
        else
            advance(1);
    }

    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) solve();
}

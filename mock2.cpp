#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

// array of booloon
// each ballon as a score
// pop all of them
// only once for each
// point: product of #left, #right (inclusive to itself)
// 1 a0, ... ,an 1
// max score
// 1. n <= 12
// 2. 1 <= score_i <= 4000

void game1() {
    int n;
    cin >> n;
    vector<i64> c(n + 2); // score
    c[0] = c[n + 1] = 1;
    for (int i = 1; i <= n; i++) cin >> c[i];
    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    i64 ans = 0;
    do {
        i64 cur = 0;
        vector<int> l(n + 2), r(n + 2);
        for (int i = 1; i <= n; i++) {
            l[i] = i - 1;
            r[i] = i + 1;
        }

        for (int x: p) {
            cur += c[l[x]] * c[x] * c[r[x]];
            l[r[x]] = l[x];
            r[l[x]] = r[x];
        }
        ans = max(ans, cur);
    } while (next_permutation(p.begin(), p.end()));
    // n! for permutation
    // O(n)
    // O(n * n!)
}

void game2() {
    int n;
    cin >> n;
    vector<i64> score(n + 2); // score
    score[0] = score[n + 1] = 1;
    for (int i = 1; i <= n; i++) cin >> score[i];
    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    i64 ans = 0;
    do {
        vector<i64> c = score;
        i64 cur = 0;
        for (int x: p) {
            cur += c[x - 1] * c[x] * c[x + 1];
            c[x] = 1;
        }
        ans = max(ans, cur);
    } while (next_permutation(p.begin(), p.end()));
    cout << ans << '\n';
    // n! for permutation
    // O(n)
    // O(n * n!)
}

// range_min
struct SegTree {
// zero-index [l, r)
#define LF 2 * v + 1
#define RT 2 * v + 2
    int n;
    vector<int> arr, add;

    SegTree(vector<int> &a) : n(a.size()), arr(4 * n), add(4 * n) {}

    void build(vector<int> &a, int v, int l, int r) {
        if (l + 1 == r) {
            arr[v] = a[l];
            return;
        }
        int m = (l + r) / 2;
        build(a, LF, l, m);
        build(a, RT, m, r);
        arr[v] = min(arr[LF], arr[RT]);
    }

    void build(vector<int> &a) { build(a, 0, 0, n); }

    // range_min
    int query_mn(int v, int l, int r, int L, int R) {
        if (L <= l && r <= R) {
            return arr[v] + add[v];
        }
        int m = (l + r) / 2;
        if (R <= m) return query_mn(LF, l, m, L, R) + add[v];
        if (m <= L) return query_mn(RT, m, r, L, R) + add[v];
        return min(query_mn(LF, l, m, L, R), query_mn(RT, m, r, L, R)) + add[v];
    }

    int query_mn(int L, int R) { return query_mn(0, 0, n, L, R); }

    // range_sum
    int range_sum(int v, int l, int r, int L, int R) {
        if (L <= l && r <= R) {
            return arr[v] + add[v] * (r - l);
        }
        int m = (l + r) / 2;
        if (R <= m) return range_sum(LF, l, m, L, R) + add[v] * (min(m, R) - l);
        if (m <= L) return range_sum(RT, m, r, L, R) + add[v] * (r - max(L, m));
        return range_sum(LF, l, m, L, R) + range_sum(RT, m, r, L, R) + add[v] * (r - l);
    }

    void update(int v, int l, int r, int L, int R, int val) {
        if (L <= l && r <= R) {
            add[v] += val;
            if (l + 1 == r) return;
        }
        int m = (l + r) / 2;
        if (L < m) update(LF, l, m, L, R, val);
        if (m < R) update(RT, m, r, L, R, val);
    }
};

void game() {
    int n;
    cin >> n;
    vector<i64> score(n + 2); // score
    vector<int> poped(n + 1);
    score[0] = score[n + 1] = 1;
    for (int i = 1; i <= n; i++) cin >> score[i];

    // [1, n]
    auto dfs = [&](auto &&dfs) -> i64 {
        i64 ans = 0;
        for (int i = 1; i <= n; i++) {
            if (poped[i] == false) {
                i64 x = score[i];
                poped[i] = true;
                score[i] = 1;
                ans = max(ans, score[x - 1] * x * score[x + 1] + dfs(dfs));
                score[i] = x;
                poped[i] = false;
            }
        }
        return ans;
    };
    cout << dfs(dfs) << '\n';
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    game();
}

#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    game();
}

// I am 4th-year student in CS computer science

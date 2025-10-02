#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 7;
int T, n, a[N], id[N], maxxn, sum, ok;
int sum_odd[N], sum_even[N];
int fa[N], ans, minn[N], maxn[N];
bool used[N];

bool cmp(int x, int y) {
    return a[x] > a[y];
}
int find(int x) {
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}
int calc_odd(int l, int r) {
    if (l % 2 == 0)
        l++;
    if (r % 2 == 0)
        r--;
    if (l > r)
        return 0;
    int u = sum_odd[r];
    if (l > 1) {
        u -= sum_odd[l - 2];
    }
    return u;
}
int calc_even(int l, int r) {
    if (l % 2 == 1)
        l++;
    if (r % 2 == 1)
        r--;
    if (l > r)
        return 0;
    int u = sum_even[r];
    u -= sum_even[l - 2];
    return u;
}
void add(int x, int y) {
    int od = maxn[x] - minn[x] + 1;
    int l = minn[x], r = maxn[x], as = 0;
    if (od % 2 == 0) {
        as += calc_odd(l, r);
        as += calc_even(l, r);
    } else {
        if (l % 2 == 1) {
            as += calc_odd(l, r);
        } else
            as += calc_even(l, r);
    }
    od = (od + 1) / 2;
    sum += y * od;
    ok += y * as;
    return;
}
void add(int i) {
    used[i] = 1;
    if (used[i - 1]) {
        int uu = find(i - 1);
        add(uu, -1);
        fa[uu] = i;
        minn[i] = min(minn[i], minn[uu]);
    }
    if (used[i + 1]) {
        int uu = find(i + 1);
        add(uu, -1);
        fa[uu] = i;
        maxn[i] = max(maxn[i], maxn[uu]);
    }
    add(i, 1);
    return;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> T;
    while (T--) {
        cin >> n;
        maxxn = 0;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
            id[i] = i;
            maxxn = max(maxxn, a[i]);
            fa[i] = i;
            minn[i] = maxn[i] = i;
        }
        sort(id + 1, id + n + 1, cmp);
        for (int i = 1; i <= n; i++) {
            if (i & 1) {
                if (i > 1) {
                    sum_odd[i] = sum_odd[i - 2];
                }
                if (a[i] == maxxn)
                    sum_odd[i]++;
            } else {
                sum_even[i] = sum_even[i - 2];
                if (a[i] == maxxn)
                    sum_even[i]++;
            }
        }
        ans = sum = ok = 0;
        for (int i = 1; i <= n; i++) {
            add(id[i]);
            if (ok)
                ans = max(ans, maxxn + a[id[i]] + sum);
            else
                ans = max(ans, maxxn + a[id[i]] + sum - 1);
        }
        cout << ans << "\n";
        for (int i = 1; i <= n; i++) {
            used[i] = 0;
            sum_even[i] = sum_odd[i] = 0;
        }
    }
    return 0;
}

// #include <bits/stdc++.h>

// using i64 = long long;
// using u64 = unsigned long long;
// using u32 = unsigned;
// template <class Info>
// struct SegmentTree {
//     int n;
//     std::vector<Info> info;
//     SegmentTree() : n(0) {}
//     SegmentTree(int n_, Info v_ = Info()) {
//         init(n_, v_);
//     }
//     template <class T>
//     SegmentTree(std::vector<T> init_) {
//         init(init_);
//     }
//     void init(int n_, Info v_ = Info()) {
//         init(std::vector(n_, v_));
//     }
//     template <class T>
//     void init(std::vector<T> init_) {
//         n = init_.size();
//         info.assign(4 << std::__lg(n), Info());
//         std::function<void(int, int, int)> build = [&](int p, int l, int r) {
//             if (r - l == 1) {
//                 info[p] = init_[l];
//                 return;
//             }
//             int m = (l + r) / 2;
//             build(2 * p, l, m);
//             build(2 * p + 1, m, r);
//             pull(p);
//         };
//         build(1, 0, n);
//     }
//     void pull(int p) {
//         info[p] = info[2 * p] + info[2 * p + 1];
//     }
//     void modify(int p, int l, int r, int x, const Info &v) {
//         if (r - l == 1) {
//             info[p] = v;
//             return;
//         }
//         int m = (l + r) / 2;
//         if (x < m) {
//             modify(2 * p, l, m, x, v);
//         } else {
//             modify(2 * p + 1, m, r, x, v);
//         }
//         pull(p);
//     }
//     void modify(int p, const Info &v) {
//         modify(1, 0, n, p, v);
//     }
//     Info rangeQuery(int p, int l, int r, int x, int y) {
//         if (l >= y || r <= x) {
//             return Info();
//         }
//         if (l >= x && r <= y) {
//             return info[p];
//         }
//         int m = (l + r) / 2;
//         return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
//     }
//     Info rangeQuery(int l, int r) {
//         return rangeQuery(1, 0, n, l, r);
//     }
//     template <class F>
//     int findFirst(int p, int l, int r, int x, int y, F &&pred) {
//         if (l >= y || r <= x) {
//             return -1;
//         }
//         if (l >= x && r <= y && !pred(info[p])) {
//             return -1;
//         }
//         if (r - l == 1) {
//             return l;
//         }
//         int m = (l + r) / 2;
//         int res = findFirst(2 * p, l, m, x, y, pred);
//         if (res == -1) {
//             res = findFirst(2 * p + 1, m, r, x, y, pred);
//         }
//         return res;
//     }
//     template <class F>
//     int findFirst(int l, int r, F &&pred) {
//         return findFirst(1, 0, n, l, r, pred);
//     }
//     template <class F>
//     int findLast(int p, int l, int r, int x, int y, F &&pred) {
//         if (l >= y || r <= x) {
//             return -1;
//         }
//         if (l >= x && r <= y && !pred(info[p])) {
//             return -1;
//         }
//         if (r - l == 1) {
//             return l;
//         }
//         int m = (l + r) / 2;
//         int res = findLast(2 * p + 1, m, r, x, y, pred);
//         if (res == -1) {
//             res = findLast(2 * p, l, m, x, y, pred);
//         }
//         return res;
//     }
//     template <class F>
//     int findLast(int l, int r, F &&pred) {
//         return findLast(1, 0, n, l, r, pred);
//     }
// };

// constexpr i64 inf = 1E18;
// struct Matrix {
//     i64 a[4][4];
//     Matrix() {
//         for (int i = 0; i < 4; i++) {
//             for (int j = 0; j < 4; j++) {
//                 a[i][j] = (i == j ? 0 : -inf);
//             }
//         }
//     }
// };

// Matrix operator+(const Matrix &a, const Matrix &b) {
//     Matrix c;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             c.a[i][j] = -inf;
//             for (int k = 0; k < 4; k++) {
//                 c.a[i][j] = std::max(c.a[i][j], a.a[i][k] + b.a[k][j]);
//             }
//         }
//     }
//     return c;
// }
// void solve() {
//     int n;
//     std::cin >> n;

//     std::vector<int> a(n);
//     for (int i = 0; i < n; i++) {
//         std::cin >> a[i];
//     }

//     std::vector<int> ord(n);
//     std::iota(ord.begin(), ord.end(), 0);
//     std::sort(ord.begin(), ord.end(),
//               [&](int i, int j) {
//                   return a[i] > a[j];
//               });

//     Matrix init;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             init.a[i][j] = -inf;
//         }
//     }
//     init.a[0][0] = 0;
//     init.a[1][0] = 0;
//     init.a[2][2] = 0;
//     init.a[3][2] = 0;
//     SegmentTree<Matrix> seg(std::vector(n, init));
//     i64 ans = 0;
//     for (auto i : ord) {
//         Matrix m = init;
//         m.a[0][1] = 1;
//         m.a[0][3] = a[i] + 1;
//         m.a[2][3] = 1;
//         seg.modify(i, m);
//         auto res = seg.rangeQuery(0, n);
//         ans = std::max({ans, res.a[0][2] + a[i], res.a[0][3] + a[i]});
//     }

//     std::cout << ans << "\n";
// }

// int main() {
//     std::ios::sync_with_stdio(false);
//     std::cin.tie(nullptr);

//     int t;
//     std::cin >> t;

//     while (t--) {
//         solve();
//     }

//     return 0;
// }
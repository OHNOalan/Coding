using i64 = long long;
using ll = long long;
using i128 = __int128;
using u128 = unsigned __int128;
using u32 = unsigned;
using u64 = unsigned long long;
using f32 = double;
using f64 = long double;

#define uset unordered_set
#define umap unordered_map
#define vs vector<string>
#define vi vector<int>
#define vvi vector<vi>
#define vvvi vector<vvi>
#define vll vector<i64>
#define vvll vector<vll>
#define vvvll vector<vvll>
#define pii pair<int, int>
#define pll pair<i64, i64>
#define vpii vector<pii>
#define vpll vector<pll>
#define vvpii vector<vpii>
#define vvpll vector<vpll>
#define vz vector<Z>
#define vvz vector<vz>
#define vvvz vector<vvz>
#define pb push_back
#define eb emplace_back

#define ALL(x) (x).begin(), (x).end()
#define DIS(x) (x).erase(unique((x).begin(), (x).end()), (x).end())
#define SRT(x) sort((x).begin(), (x).end())
#define REV(x) reverse((x).begin(), (x).end())
#define FND(x, y) find((x).begin(), (x).end(), (y))
#define CNT(x, y) count((x).begin(), (x).end(), (y))
#define SIZ(x) (static_cast<int>(x.size()))
#define MIN(x) *min_element((x).begin(), (x).end())
#define MAX(x) *max_element((x).begin(), (x).end())
#define LWB(x, y) distance((x).begin(), lower_bound((x).begin(), (x).end(), (y)))
#define YES(x) cout << ((x) ? "YES\n" : "NO\n")
#define yes(x) cout << ((x) ? "yes\n" : "no\n")
#define Yes(x) cout << ((x) ? "Yes\n" : "No\n")

#define rep(i, x, y) for (int(i) = (x); (i) < (y); (i)++)
#define reps(i, x, y, s) for (int(i) = (x); (i) < (y); (i) += (s))
#define repr(i, x, y) for (int(i) = (x); (i) > (y); (i)--)
#define reprs(i, x, y, s) for (int(i) = (x); (i) > (y); (i) -= (s))
#define fi first
#define se second

#include <bits/stdc++.h>

using namespace std;


template<class Info>
struct SegmentTree {
    int n;
    std::vector<Info> info;
    SegmentTree() : n(0) {}
    SegmentTree(int n_, Info v_ = Info()) { init(n_, v_); }
    template<class T>
    SegmentTree(std::vector<T> init_) {
        init(init_);
    }
    void init(int n_, Info v_ = Info()) { init(std::vector(n_, v_)); }
    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        info.assign(4 << (std::bit_width<unsigned int>(n) - 1), Info());
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }
    void pull(int p) { info[p] = info[2 * p] + info[2 * p + 1]; }
    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }
    void modify(int p, const Info &v) { modify(1, 0, n, p, v); }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) {
            return Info();
        }
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
    }
    Info rangeQuery(int l, int r) { return rangeQuery(1, 0, n, l, r); }
    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F &&pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(2 * p + 1, m, r, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findFirst(int l, int r, F &&pred) {
        return findFirst(1, 0, n, l, r, pred);
    }
    template<class F>
    int findLast(int p, int l, int r, int x, int y, F &&pred) {
        if (l >= y || r <= x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        int res = findLast(2 * p + 1, m, r, x, y, pred);
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findLast(int l, int r, F &&pred) {
        return findLast(1, 0, n, l, r, pred);
    }
};

struct Info {
    int x[2]{};
    int c[2]{};
};


Info operator+(Info l, const Info &r) {
    for (int i = 0; i < 2; i++) {
        int x = r.x[i];
        int c = r.c[i];
        if (l.c[0] && l.x[0] == x) {
            l.c[0] += c;
            continue;
        }
        if (l.c[1] && l.x[1] == x) {
            l.c[1] += c;
            continue;
        }
        int t = min({l.c[0], l.c[1], c});
        l.c[0] -= t;
        l.c[1] -= t;
        c -= t;
        if (c) {
            if (!l.c[0]) {
                l.x[0] = x;
                l.c[0] = c;
            } else {
                l.x[1] = x;
                l.c[1] = c;
            }
        }
    }
    return l;
}


void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int &x: a) cin >> x;

    auto val = a;
    std::sort(val.begin(), val.end());
    for (int i = 0; i < n; i++) { a[i] = std::lower_bound(val.begin(), val.end(), a[i]) - val.begin(); }

    std::vector<std::vector<int>> vec(n);
    for (int i = 0; i < n; i++) { vec[a[i]].push_back(i); }

    SegmentTree<Info> seg(n);
    for (int i = 0; i < n; i++) {
        Info x;
        x.x[0] = a[i];
        x.c[0] = 1;
        seg.modify(i, x);
    }

    for (int i = 0; i < q; i++) {
        int l, r;
        std::cin >> l >> r;
        l--;

        auto res = seg.rangeQuery(l, r);
        std::vector<int> ans;
        for (int j = 0; j < 2; j++) {
            if (res.c[j]) {
                int x = res.x[j];
                int lo = std::lower_bound(vec[x].begin(), vec[x].end(), l) - vec[x].begin();
                int hi = std::lower_bound(vec[x].begin(), vec[x].end(), r) - vec[x].begin();
                if (hi - lo > (r - l) / 3) {
                    ans.push_back(val[x]);
                }
            }
        }
        std::sort(ans.begin(), ans.end());
        for (int i = 0; i < ans.size(); i++) { std::cout << ans[i] << " \n"[i == ans.size() - 1]; }
        if (ans.empty()) {
            std::cout << -1 << "\n";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t = 1;
    if (true)
        cin >> t;
    while (t--) solve();
    return 0;
}

#include <bits/stdc++.h>

#define pb emplace_back
#define mp make_pair
#define x first
#define y second
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

typedef long double ld;
typedef long long ll;

using namespace std;

mt19937 rnd(time(nullptr));

const int inf = 1e9;
const int M = 1e9 + 7;
const ld pi = atan2(0, -1);
const ld eps = 1e-6;
int init = []() -> int { return 0; }();

// 根据题目用 FenwickTree<int> t(n) 或者 FenwickTree<long long> t(n) 初始化
template<typename T>
class FenwickTree {
    vector<T> tree;

public:
    // 使用下标 1 到 n
    FenwickTree(int n) : tree(n + 1) {}

    // a[i] 增加 val
    // 1 <= i <= n
    // 时间复杂度 O(log n)
    void update(int i, T val) {
        for (; i < tree.size(); i += i & -i) {
            tree[i] += val;
        }
    }

    // 求前缀和 a[1] + ... + a[i]
    // 1 <= i <= n
    // 时间复杂度 O(log n)
    T pre(int i) const {
        T res = 0;
        for (; i > 0; i &= i - 1) {
            res += tree[i];
        }
        return res;
    }

    // 求区间和 a[l] + ... + a[r]
    // 1 <= l <= r <= n
    // 时间复杂度 O(log n)
    T query(int l, int r) const {
        if (r < l) {
            return 0;
        }
        return pre(r) - pre(l - 1);
    }
};

ll f(const vector<int> &a, int n) {
    FenwickTree<int> t(n);
    ll ans = 0;
    for (int i = 0; i < a.size(); i++) {
        ans += t.query(a[i] + 1, n);
        t.update(a[i], 1);
    }
    return ans % 2;
}

void solve() {
    int n;
    cin >> n;
    vector<int> a, b;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        if (i & 1)
            b.push_back(x);
        else
            a.push_back(x);
    }
    bool v = f(a, n) != f(b, n);
    sort(all(a));
    sort(all(b));
    vector<int> ans(n);
    for (int i = 0; i < n; i++) {
        ans[i] = (i & 1) ? b[i / 2] : a[i / 2];
    }
    if (v)
        swap(ans[n - 3], ans[n - 1]);
    for (int x: ans)
        cout << x << " ";
    cout << endl;
}


#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
        // cout << "\n";
    }
    return 0;
}

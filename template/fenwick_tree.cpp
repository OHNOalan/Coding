#include <bits/stdc++.h>
using namespace std;

#if defined(__clang__)
template<typename _Tp>
_Tp __lg(_Tp __n)
{
    return (sizeof(+__n) * __CHAR_BIT__ - 1)
           - (sizeof(+__n) == sizeof(long long)
            ? __builtin_clzll(+__n)
            : (sizeof(+__n) == sizeof(long)
             ? __builtin_clzl(+__n)
             : __builtin_clz(+__n)));
}
#endif

template <typename T>
struct Fenwick {
    int n;
    std::vector<T> a;

    Fenwick(int n_ = 0) {
        init(n_);
    }

    void init(int n_) {
        n = n_;
        a.assign(n, T{});
    }

    void add(int x, const T &v) {
        for (int i = x + 1; i <= n; i += i & -i) {
            a[i - 1] = a[i - 1] + v;
        }
    }

    T sum(int x) {
        T ans{};
        for (int i = x; i > 0; i -= i & -i) {
            ans = ans + a[i - 1];
        }
        return ans;
    }

    T rangeSum(int l, int r) {
        return sum(r) - sum(l);
    }

    int select(const T &k) {
        int x = 0;
        T cur{};
        for (int i = 1 << __lg(n); i; i /= 2) {
            if (x + i <= n && cur + a[x + i - 1] <= k) {
                x += i;
                cur = cur + a[x - 1];
            }
        }
        return x;
    }
};
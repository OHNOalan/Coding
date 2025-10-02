#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define ull unsigned long long
#define i128 __int128

ll read() {
    ll x = 0;
    bool f = false;
    char c = getchar();
    while (c < '0' || c > '9') f |= (c == '-'), c = getchar();
    while (c >= '0' && c <= '9') x = (x << 1) + (x << 3) + (c & 15), c = getchar();
    return f ? -x : x;
}

const ll inf = 0x3f3f3f3f3f3f3f;
const int N = 425, M = N * N * N / 6;
int n;
ll a[N];
// vector< vector<ll> > f[N], g[N];
ll f[M], g[M];

ll sum(int l, int r) { return a[r] - a[l - 1]; }

ll ub(int l, int r, int m) { return abs(a[m] - a[l - 1] - (a[r] - a[m])); }

ll &get(ll a[], int l, int r, int m) { return a[r * (r - 1) * (r - 2) / 6 + m * (m - 1) / 2 + l]; }

void solve() {
    n = read();
    for (int i = 1; i <= n; ++i) a[i] = a[i - 1] + read();
    for (int l = 1; l <= n; ++l)
        for (int r = l + 1; r <= n; ++r)
            for (int m = l; m < r; ++m) {
                get(f, l, r, m) = get(g, l, r, m) = inf;
                if (l == m)
                    get(f, l, r, m) = 0;
                if (m + 1 == r)
                    get(g, l, r, m) = 0;
            }
    // return ;
    for (int len = 2; len <= n; ++len)
        for (int l = 1, r = len; r <= n; ++l, ++r) {
            ll val = (64 - __builtin_clzll(sum(l, r) - 1));
            for (int m = l; m < r; ++m)
                get(f, l, r, m) = min(inf, get(f, l, r, m) + get(g, l, r, m) + val * min(sum(l, m), sum(m + 1, r)));
            // 更新f
            int p, q;
            for (p = l; p < r && sum(l, p) < sum(p + 1, r); ++p);
            for (q = r + 1; q <= n && sum(l, r) > sum(r + 1, q); ++q);
            ll mn = inf;
            for (int k = q - 1, L = p - 1, R = p; k >= r + 1; --k) {
                while (L >= l && ub(l, r, L) <= ub(l, k, r)) mn = min(mn, get(f, l, r, L)), --L;
                while (R < r && ub(l, r, R) <= ub(l, k, r)) mn = min(mn, get(f, l, r, R)), ++R;
                get(f, l, k, r) = min(get(f, l, k, r), mn);
            }
            mn = inf;
            for (int k = q, L = p - 1, R = p; k <= n; ++k) {
                while (L >= l && ub(l, r, L) <= ub(l, k, r)) mn = min(mn, get(f, l, r, L)), --L;
                while (R < r && ub(l, r, R) <= ub(l, k, r)) mn = min(mn, get(f, l, r, R)), ++R;
                get(f, l, k, r) = min(get(f, l, k, r), mn);
            }
            // 更新g
            for (q = l - 1; q >= 1 && sum(l, r) > sum(q, l - 1); --q);
            mn = inf;
            for (int k = q + 1, L = p - 1, R = p; k < l; ++k) {
                while (L >= l && ub(l, r, L) <= ub(k, r, l - 1)) mn = min(mn, get(f, l, r, L)), --L;
                while (R < r && ub(l, r, R) <= ub(k, r, l - 1)) mn = min(mn, get(f, l, r, R)), ++R;
                get(g, k, r, l - 1) = min(get(g, k, r, l - 1), mn);
            }
            mn = inf;
            for (int k = q, L = p - 1, R = p; k >= 1; --k) {
                while (L >= l && ub(l, r, L) <= ub(k, r, l - 1)) mn = min(mn, get(f, l, r, L)), --L;
                while (R < r && ub(l, r, R) <= ub(k, r, l - 1)) mn = min(mn, get(f, l, r, R)), ++R;
                get(g, k, r, l - 1) = min(get(g, k, r, l - 1), mn);
            }
        }
    for (int i = 1; i < n; ++i) {
        if (get(f, 1, n, i) >= inf)
            printf("-1 ");
        else
            printf("%lld ", get(f, 1, n, i));
    }
    printf("\n");
}

int main() {
    int T = read();
    while (T--) solve();
    return 0;
}

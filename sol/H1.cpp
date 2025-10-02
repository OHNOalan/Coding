#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

using u64 = unsigned long long;

constexpr u64 MX = ULLONG_MAX;
constexpr int B = 16;
const int maxn = 1e6 + 7;

int a[maxn];

int bk[1 << 16], pre[1 << 16];
long long sum[1 << 16];

void init() {
    for (int i = 0; i < (1 << B); i++) {
        for (int j = 0; j < 16; j++) {
            if ((i >> j) & 1) {
                break;
            } else {
                pre[i]++;
            }
        }
        for (int j = 15; j >= 0; j--) {
            if (i >> j & 1) {
                break;
            } else {
                bk[i]++;
            }
        }
        int cur = 0;
        for (int j = 0; j < 16; j++) {
            if (i >> j & 1) {
                cur = 0;
            } else {
                cur++;
            }
            sum[i] += cur;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    init();

    int mx = (1 << 16) - 1;
    int n, q;
    cin >> n >> q;
    std::vector<u64> a(n);
    string s;
    cin >> s;
    for (int i = 0; i < n; i++) { a[i] = s[i] - '0'; }

    const int N = (n + 63) / 64;
    vector<u64> b(N + 1);

    auto get = [&](int x) {
        int p = x / 64;
        int r = x % 64;
        int q = p + 1;
        u64 res = 0;
        res = (b[p] >> r) | (b[q] << (64 - r));
        return res;
    };

    for (int i = 0; i < n; i++) { b[i / 64] |= (a[i] << (i % 64)); }

    vector<u64> lobit(65), hibit(65);
    for (int i = 0; i < 64; i++) {
        lobit[i + 1] = lobit[i] | (1ull << i);
        hibit[i + 1] = hibit[i] | (1ull << (63 - i));
    }

    vector<int> flip(n);
    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int l, r;
            cin >> l >> r;
            l--;
            r--;
            int bl = l / 64, br = r / 64;
            if (bl == br) { // 同一个块内
                b[bl] ^= (lobit[r - l + 1] << (l - bl * 64));
            } else {
                b[bl] ^= (hibit[bl * 64 + 64 - l]);
                b[br] ^= (lobit[r - br * 64 + 1]);
                flip[bl + 1] ^= 1;
                flip[br] ^= 1;
            }
        } else {
            int t, x, y;
            cin >> t >> x >> y;
            x--, y--;
            // 处理之前的翻转
            int v = 0;
            for (int i = 0; i < N; i++) {
                v ^= flip[i];
                if (v) {
                    b[i] ^= MX;
                }
                flip[i] = 0;
            }
            // 计算结果
            long long ans = 0;
            int cur = 0;
            int p = t / 64;
            int q = t % 64;
            for (int i = 0; i < p; i++) {
                auto u = get(x) ^ get(y);
                for (int j = 0; j < 4; j++) {
                    u64 v = (mx & u);
                    u >>= 16;
                    ans += sum[v] + 1ll * cur * pre[v];
                    if (bk[v] == 16)
                        cur += 16;
                    else
                        cur = bk[v];
                }
                x += 64, y += 64;
            }
            if (q) {
                auto u = get(x) ^ get(y);
                for (int i = 0; i < q; i++) {
                    if ((u >> i) & 1)
                        cur = 0;
                    else
                        cur++;
                    ans += cur;
                }
            }
            cout << ans << "\n";
        }
    }

    return 0;
}

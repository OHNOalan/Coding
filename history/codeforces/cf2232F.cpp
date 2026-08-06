#include <bits/stdc++.h>

using namespace std;

// 解决 C++11 std::gcd 不支持 __int128 的问题
__int128 my_gcd(__int128 a, __int128 b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// 扩展欧几里得算法，用于求逆元
__int128 exgcd(__int128 a, __int128 b, __int128 &x, __int128 &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    __int128 x1, y1;
    __int128 d = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return d;
}

// 模拟 Python 的 % 运算符（确保结果为非负数）
inline __int128 safe_mod(__int128 x, __int128 m) {
    __int128 res = x % m;
    if (res < 0) res += m;
    return res;
}

// 模拟 Python 的 pow(d, -1, f)，即求 d 模 f 的逆元
// 注意：传入的 d 和 f 可能是负数，内部做了标准化处理
__int128 modInverse(__int128 d, __int128 f) {
    d = safe_mod(d, f); // match Python's pow(d, -1, f): reduce d to [0, f) first
    __int128 x, y;
    __int128 g = exgcd(d, f, x, y);
    if (g != 1) return -1;
    __int128 res = (x % f + f) % f;
    return res;
}

// 针对 __int128 的快读
inline __int128 read() {
    __int128 x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

// 针对 __int128 的快写
inline void write(__int128 x) {
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    if (x > 9) write(x / 10);
    putchar(x % 10 + '0');
}

__int128 solve() {
    __int128 n = read();
    __int128 a = read();
    __int128 b = read();
    __int128 k = read();

    __int128 g = my_gcd(a, b);
    if (k % g > 0) return 0;

    a /= g;
    b /= g;
    k /= g;
    swap(a, b);

    if (a == b) return n;

    __int128 inv_a = modInverse(a, b);
    __int128 mx = safe_mod(k * inv_a, b);
    if (mx * a > k) return 0;

    __int128 first = -1;
    __int128 x = 0;
    while (true) {
        first += 1;
        if (safe_mod(k - a * x, b) > 0) break;

        // Python 的 // 是向下取整，但由于上面一行已经保证了能整除，
        // 且这里的 (k - a * x) 在整除时，C++ 的 / 和 Python 的 // 结果完全一致。
        x = (k - a * x) / b;
        if (x < 0) break;
    }

    if (first >= n) return n;
    if (k % (a + b) == 0) return n - 1;

    if (a > b) swap(a, b);

    __int128 per = 0;
    __int128 d = 1, e = 0, f = 1;

    while (true) {
        per += 1;

        // 对应 Python 的元组更新：d, e, f = -a*d, f*k - a*e, b*f
        __int128 next_d = -a * d;
        __int128 next_e = f * k - a * e;
        __int128 next_f = b * f;
        d = next_d;
        e = next_e;
        f = next_f;

        g = my_gcd(f, d);
        if (safe_mod(e, g) > 0) break;

        d /= g;
        e /= g;
        f /= g;

        __int128 inv_d = modInverse(d, f);
        if (inv_d == -1) break; // 防御性逻辑，如果不存在逆元则跳出

        mx = safe_mod((-e) * inv_d, f);
        if (mx * a > k) break;
    }

    if (first >= n) return n;
    __int128 ans = n - 1;
    ans -= (n - first - 1) / per;

    return ans;
}

int main() {
    // 提高读写效率
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (scanf("%d", &t) != EOF) {
        while (t--) {
            write(solve());
            putchar('\n');
        }
    }
    return 0;
}

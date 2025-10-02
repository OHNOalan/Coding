#include <bits/stdc++.h>
using namespace std;
static constexpr long long MOD = 998244353;

template<typename T>
T pow(T a, T b) {
    T ret = 1;
    while (b) {
        if (b & 1) {
            ret = (long long) ret * a % MOD;
        }
        a = (long long) a * a % MOD;
        b >>= 1;
    }
    return ret;
}
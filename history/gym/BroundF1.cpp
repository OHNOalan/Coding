#include <bits/stdc++.h>
using namespace std;
#define MAXN 1000
#define MOD 998244353
#define MOD2 1'000'000'007

using ll = long long;

ll exgcd(ll a, ll b, ll& x, ll& y)
{
    if (b == 0)
    {
        x = 1, y = 0;
        return a;
    }
    ll ret = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return ret;
}
ll Inv(ll a, int mod=MOD)
{
    ll x, y;
    ll d = exgcd(a, mod, x, y);
    return d == 1 ? (x % mod + mod) % mod : -1;
}

// int sg[MAXN];
//
// void init() {
//     memset(sg, 0, sizeof(sg));
//     sg[0] = 0;
//     vector<vector<int>> res(100);
//     for(int i=1; i<200; i++) {
//         set<int> s;
//         for(int j=1; j<i; j++) {
//             if(i % j == 0) {
//                 s.insert(sg[i-j]);
//             }
//         }
//         for(int mex=0; mex<MAXN + 10; mex++) {
//             if(s.find(mex) == s.end()) {
//                 sg[i] = mex;
//                 res[mex].push_back(i);
//                 break;
//             }
//         }
//     }
//     for(int mex=0; mex<=10; mex++) {
//         cout << mex << ": ";
//         for(int x : res[mex]) {
//             cout << bitset<16>(x) << " ";
//         }
//         cout << endl;
//     }
// }

void solve() {
    int n; cin >> n;
    int ans = 0;
    for(int i=0; i<n; i++) {
        int x; cin >> x;
        ans ^= __builtin_ctz(x & -x);
    }
    cout << (ans ? "YES\n" : "NO\n");
}

#define MULTICASE true
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
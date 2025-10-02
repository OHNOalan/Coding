#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 1e7 + 10;

using ll = long long;

int dpf[MAX];

void init() {
    iota(dpf, dpf + MAX, 0);
    for (int i = 2; i < MAX; i++)
        if (dpf[i] == i)
            for (int j = i + i; j < MAX; j += i) {
                dpf[j] = min(dpf[j], i);
            }
}

int sg(int k) {
    if (k == 1) return 1;
    if (dpf[k] == k) return 0;
    int ans = 0;
    while (k > 1) ans++, k /= dpf[k];
    return ans;
}

void solve() {
    int n;
    cin >> n;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        ans ^= sg(x);
    }
    cout << (ans ? "YES\n" : "NO\n");
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

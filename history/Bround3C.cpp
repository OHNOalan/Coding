#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 1e7 + 10;

using ll = long long;

int cal(int n) {
    return (int)sqrt(n) + (int)sqrt(n/2);
}

void solve() {
    int l, r; cin >> l >> r;
    cout << cal(r) - cal(l-1) << '\n';
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
    }
    return 0;
}

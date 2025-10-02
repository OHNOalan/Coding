#include <bits/stdc++.h>
using namespace std;
int init = []() -> int { return 0; }();

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    int ans = 1, pre = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] <= pre + 1)
            pre = min(pre, a[i]);
        else {
            ans += 1;
            pre = a[i];
        }
    }
    cout << ans << '\n';
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

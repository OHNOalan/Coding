#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void init() {}

void solve() {
    int n; cin >> n;
    vector<int> a(n);
    for(int i=0; i<n; i++) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    int ans = n;
    for (int i = 0, j = 0; i < n - 1; i++) {
        while (j + 1 < n && a[i] + a[i + 1] > a[j + 1]) j++;
        ans = min(ans, i + n - 1 - j);
    }
    cout << ans << '\n';
}

#define MULTICASE true
int main() {
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
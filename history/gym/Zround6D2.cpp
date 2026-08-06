#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    vector<int> a(n+1), b(n+1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];
    vector<int> nums(n), d(n+1);
    for(int i=1; i<=n; i++) {
        d[a[i]] = i;
    }
    for(int i=1; i<=n; i++) {
        nums[i-1] = d[b[i]];
    }
    auto end = nums.begin();
    for (int x : nums) {
        auto it = lower_bound(nums.begin(), end, x);
        *it = x;
        if (it == end) {
            ++end;
        }
    }
    cout << end - nums.begin() << '\n';
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

#include <bits/stdc++.h>
using namespace std;
int init = []() -> int { return 0; }();

void solve() {
    int n;
    cin >> n;
    vector lo(2, vector<int>(2, INT_MAX)); //[[min_x, min2_x], [min_y, min2_y]]
    vector hi(2, vector<int>(2, INT_MIN)); //[[max_x, max2_x], [max_y, max2_y]]
    vector ps(n, vector<int>(2));
    for (int i = 0, x; i < n; i++)
        for (int d = 0; d <= 1; d++) {
            cin >> x;
            ps[i][d] = x;
            if (x < lo[d][0]) {
                lo[d][1] = lo[d][0];
                lo[d][0] = x;
            } else if (x < lo[d][1]) {
                lo[d][1] = x;
            }
            if (x > hi[d][0]) {
                hi[d][1] = hi[d][0];
                hi[d][0] = x;
            } else if (x > hi[d][1]) {
                hi[d][1] = x;
            }
        }
    long long ans = 1LL * (hi[0][0] - lo[0][0] + 1) * (hi[1][0] - lo[1][0] + 1);
    for (int i = 0; i < n; i++) {
        int diff_x = (hi[0][0] == ps[i][0] ? hi[0][1] : hi[0][0]) - (lo[0][0] == ps[i][0] ? lo[0][1] : lo[0][0]) + 1;
        int diff_y = (hi[1][0] == ps[i][1] ? hi[1][1] : hi[1][0]) - (lo[1][0] == ps[i][1] ? lo[1][1] : lo[1][0]) + 1;
        long long area = 1LL * diff_x * diff_y;
        if (area < n)
            area += min(diff_x, diff_y);
        ans = min(ans, area);
    }
    cout << ans << endl;
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

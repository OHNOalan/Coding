#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
static constexpr int MOD = 1e9 + 7;

void solve() {
    int n,m,r,c;
    cin >> n >> m >> r >> c;
    long long ans = (ll) (n-r) * (2 * m - 1) + (m - c);
    cout << ans << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
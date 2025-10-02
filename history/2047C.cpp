#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n; cin >> n;
    vector<array<ll, 2>> a(n);
    for(int i=0; i<n; i++) cin >> a[i][0];
    for(int i=0; i<n; i++) cin >> a[i][1];
    ll cost = 0, extra = INT_MIN;
    for(int i=0; i<n; i++) {
        ll mn = min(a[i][0], a[i][1]);
        ll mx = max(a[i][0], a[i][1]);
        cost += mx;
        extra = max(extra, mn);
    }
    cout << cost + extra << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
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

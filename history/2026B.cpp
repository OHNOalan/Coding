#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
static constexpr int MOD = 1e9 + 7;
static constexpr ll INF = 1e18 + 1;

void solve() {
    int n; cin >> n;
    vector<ll> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());

    // even number
    if((n & 1) == 0) {
        ll ans = 0;
        for(int i = 0; i < n; i+=2) {
            ans = max(ans, a[i+1] - a[i]);
        }
        cout << ans << '\n';
        return;
    }

    // odd number
    vector<ll> l_free(n, -1), r_free(n, -1);
    r_free[n-1] = a[n-1] + 1;
    l_free[0] = a[0] - 1;
    for(int i=0; i<n;) {
        int start = i;
        while(i<n-1 && a[i] + 1 == a[i+1]) {
            i++;
        }
        for(int j=start; j<=i; j++) {
            r_free[j] = a[i]+1;
        }
        i++;
    }
    for(int i=n-1; i>=0;) {
        int start = i;
        while(i > 0 && a[i] - 1 == a[i-1]) {
            i--;
        }
        for(int j=start; j>=i; j--) {
            l_free[j] = a[i]-1;
        }
        i--;
    }

    vector<ll> left(n), right(n);
    ll mxL = 0;
    for(int i = 2; i < n; i+=2) {
        mxL = max(mxL, a[i-1] - a[i-2]);
        left[i] = mxL;
    }

    ll mxR = 0;
    for(int i = n-3; i >= 0; i-=2) {
        mxR = max(mxR, a[i+2] - a[i+1]);
        right[i] = mxR;
    }

    ll ans = INF;
    for (int i = 0; i < n; i+=2) {
        ll extra = min(r_free[i] - a[i], a[i] - l_free[i]);
        ans = min(ans, max({left[i], right[i], extra}));
    }
    cout << ans << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
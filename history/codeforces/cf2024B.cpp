#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n, k; cin >> n >> k;
    vector<ll> a(n);
    map<ll,int> freq;
    for(int i=0; i<n; i++) {
        cin >> a[i];
        freq[a[i]]++;
    }
    ll left = n, used = 0;
    ll ans = 0;
    for(auto &[x, cnt] : freq) {
        if((x - used) * left >= k) {
            ans += k;
            cout << ans << '\n';
            return;
        }
        k -= ((x - used) * left);
        ans += (x - used) * left + cnt;
        left -= cnt;
        used = x;
    }
    cout << "damn\n" << endl;
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }

    return 0;
}
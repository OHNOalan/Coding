#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
static constexpr int MOD = 1e9 + 7;

ll quickMul(ll a, ll b) {
    int ret = 1;
    while(b) {
        if(b & 1) ret = ret * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return ret;
}

void solve() {
    int n; cin >> n;
    vector<ll> a(n), odd(n), twos(n);
    vector<ll> prefOdd(n+1), prefTwos(n+1);
    for(int i=0; i<n; i++) {
        cin >> a[i];
        int k = __builtin_ctz(a[i]);
        twos[i] = k;
        odd[i] = a[i] >> k;
        prefTwos[i+1] = prefTwos[i] + twos[i];
        prefOdd[i+1] = prefOdd[i] + odd[i];
    }
    stack<array<ll,2>> st;
    ll ans = 0;
    st.push({-1,0});
    for(int i=0; i<n; i++) {
        ll x = odd[i];
        while(st.size() > 1 && odd[st.top()[0]] <= (x << (prefTwos[i+1] - prefTwos[st.top()[0] + 1]))) {
            ll diff = st.top()[1];
            ans = (ans - diff + MOD) % MOD;
            st.pop();
        }
        ll j = st.top()[0];
        ll ts = prefTwos[i+1] - prefTwos[j + 1];
        ll sums = prefOdd[i+1] - prefOdd[j + 1];
        ll diff = sums + x * (quickMul(2, ts) - 1) % MOD;
        st.push({i,diff});
        ans = (ans + diff) % MOD;
        cout << ans << " ";
    }
    cout << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
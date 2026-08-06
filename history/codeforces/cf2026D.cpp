#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
static constexpr int MOD = 1e9 + 7;
static constexpr ll INF = 1e18 + 1;


int main() {
    ll n; cin >> n;

    vector<ll> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    vector<ll> pre(n+1);
    for (int i = 0; i < n; i++)
        pre[i+1] = pre[i] + a[i];

    vector<ll> s0(n+2), s1(n+2), ss(n+2);
    for (int i = 0; i <= n; i++) {
        s0[i+1] = s0[i] + pre[i];
        s1[i+1] = s1[i] + pre[i] * i;
    }

    for (int i = 0; i <= n; i++) {
        ss[i+1] = ss[i] + s0[i];
    }

    auto query = [&](ll x) -> ll {
        ll lo = 0, hi = n+1;
        while(lo + 1 < hi) {
            ll mid = (lo + hi) / 2;
            (x >= (n + n - mid + 1) * mid / 2 ? lo : hi) = mid;
        }

        ll res = 0;

        ll u = lo;
        res += s0[n+1] * u;
        res -= ss[u];
        res -= s0[u] * (n+1);
        res += s1[u];
        x -= (n + n - u + 1) * u / 2;
        res += s0[u + x + 1] - s0[u];
        res -= pre[u] * (x + 1);
        return res;
    };

    int q; cin >> q;
    while(q-- && q > -3) {
        ll l, r; cin >> l >> r;
        l--;
        cout << query(r) - query(l) << '\n';
    }
    return 0;
}
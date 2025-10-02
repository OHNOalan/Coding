#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    vector<int> a(n);
    for(int i=0; i<n; i++) {
        cin >> a[i];
    }
    set<int> s;
    for(int i=0; i<n; i++) {
        int x;
        cin >> x;
        a[i] -= x;
        s.insert(a[i]);
        s.insert(-a[i]);
    }
    vector<int> tr;
    int pos = 0;
    map<int, int> mp;
    for(int x : s) {
        mp[x] = pos++;
    }
    auto lowbit = [&] (int x) -> int { return x & -x ;};

    auto add = [&] (int x, int v) -> void
    {
        for (int i = x; i <= n; i += lowbit(i)) tr[i] += v;
    };

    auto query = [&] (int x) -> int
    {
        int res = 0;
        for (int i = x; i; i -= lowbit(i)) res += tr[i];
        return res;
    };
    ll ans = 0;
    for(int j=0; j<n; j++) {
        int x = a[j];
        ans += query(mp[-x]);
        add(mp[x], 1);
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

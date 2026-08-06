#include <bits/stdc++.h>
using namespace std;

using ll = long long;
void solve() {
    int n; cin >> n;
    vector<int> a(n), id(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](const int i, const int j) {
        return a[i] < a[j];
    });
    sort(a.begin(), a.end());
    int mx = 1, cnt = 0, pre = 0;
    for(int x : a) {
        if(x == pre) {
            cnt++;
        } else {
            pre = x;
            cnt = 1;
        }
        mx = max(mx, cnt);
    }
    if(mx * 2 > n) {
        cout << "0\n";
        return;
    }
    vector<int> b(n);
    for(int i = 0; i < n; i++) {
        b[(i + mx) % n] = a[i];
    }
    for(int i=0; i<n; i++) {
        a[id[i]] = b[i];
    }
    for(int x : a) cout << x << " "; cout << '\n';
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

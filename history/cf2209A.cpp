#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, c;
    long long k;
    cin >> n >> c >> k;
    long long ans = c;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    sort(a.begin(), a.end());
    for (int x: a) {
        if (ans < x) break;
        long long diff = ans - x;
        diff = min(diff, k);
        ans += x + diff;
        k -= diff;
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

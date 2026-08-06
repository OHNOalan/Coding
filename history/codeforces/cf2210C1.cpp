#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

vector<ll> pri = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
int L = pri.size();

void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 2), b(n + 1);
    a[0] = a[n + 1] = 1;
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        int d = lcm(gcd(a[i], a[i - 1]), gcd(a[i], a[i + 1]));
        if (d <= b[i] && d != a[i]) ans++;
    }
    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) cin >> v[i];
    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> a[i][j];
        }
    }
    int ans = m;
    long long sum = 0;
    priority_queue<int, vector<int>, greater<>> pq;
    for (int r = n - 1; r >= 0 && ans > 1; --r) {
        int k = ans - 1;
        for (int c = 0; c < m; ++c) {
            pq.push(a[r][c]);
            sum += a[r][c];
        }
        while (pq.size() > k) {
            sum -= pq.top();
            pq.pop();
        }
        while (sum >= v[r]) {
            ans = min(ans, (int) pq.size());
            sum -= pq.top();
            pq.pop();
        }
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> xs(n), ys(n), U(n + 1), D(n + 1), u(n), d(n);
    for (int i = 0; i < n; i++) {
        cin >> xs[i] >> ys[i];
        --xs[i], --ys[i];
        U[xs[i] + 1] = D[ys[i] + 1] = 1;
    }
    for (int i = 1; i <= n; i++) {
        U[i] += U[i - 1];
        D[i] += D[i - 1];
    }
    for (int i = 0; i < n; i++) {
        xs[i] = U[xs[i]];
        ys[i] = D[ys[i]];
    }
    fill(u.begin(), u.end(), 0);
    fill(d.begin(), d.end(), n);
    fill(U.begin(), U.end(), 0);
    fill(D.begin(), D.end(), n);
    for (int i = 0; i < n; i++) u[xs[i]] = max(u[xs[i]], ys[i]);
    for (int i = 0; i < n; i++) d[xs[i]] = min(d[xs[i]], ys[i]);
    for (int i = 0; i < n; i++) U[i + 1] = max(U[i], u[i]);
    for (int i = 0; i < n; i++) D[i + 1] = min(D[i], d[i]);

    int ru = 0, rd = n;
    long long ans = 0;
    for (int i = n - 1; i > 0; --i) {
        ru = max(ru, u[i]);
        rd = min(rd, d[i]);
        ans += max(0, min(U[i], ru) - max(D[i], rd));
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

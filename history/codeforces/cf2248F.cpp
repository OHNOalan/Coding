#include <bits/stdc++.h>

using namespace std;

long long ceil(long long a, long long b) { return (a + b - 1) / b; }

constexpr long long INF = LLONG_MIN / 2;

void solve() {
    int n, m, k, x;
    cin >> n >> m >> k;
    vector<long long> v(n * m);
    vector<long long> sumR(n), sumC(m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> x;
            v[i * m + j] = x;
            sumR[i] += x;
            sumC[j] += x;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            v[i * m + j] = sumR[i] + sumC[j] - 3 * v[i * m + j];
        }
    }

    if (n >= 2 && m >= 2) {
        nth_element(v.begin(), v.begin() + k - 1, v.end());
        long long d = max(0ll, v[k - 1]);
        long long inc = n + m - 3;
        long long ops = (d + inc - 1) / inc;
        cout << ops << '\n';
        return;
    }
    long long sum = (n == 1) ? sumR[0] : sumC[0];
    n = n * m;
    long long ans;
    if (n == 1) {
        ans = v[0] <= 0 ? 0 : -1;
    } else if (n == 2) {
        if (k == 1)
            ans = 0;
        else
            ans = max(v[0], v[1]);
    } else {
        vector<long long> all = v;
        sort(all.begin(), all.end());
        ans = max(0ll, ceil(all[k - 1], n - 2));
        auto calc = [&](long long x, vector<long long> &a) {
            if (k > 1) nth_element(a.begin(), a.begin() + k - 2, a.end());
            long long s = (k > 1) ? a[k - 2] : INF;
            return max({
                    0ll,
                    ceil(x, n - 1),
                    s == INF ? 0ll : ceil(s, n - 2),
                    s == INF ? 0ll : ceil(x + s, 2 * n - 4),
            });
        };
        vector<long long> R(v.begin() + 1, v.end());
        ans = min(ans, calc(v[0], R));
        vector<long long> L(v.begin(), v.end() - 1);
        ans = min(ans, calc(v[n - 1], L));
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

#include <bits/stdc++.h>

using namespace std;

void solve() {
    long long n, d;
    int m;
    cin >> n >> m >> d;
    vector<long long> p(m), r(m);
    for (int i = 0; i < m; i++) cin >> p[i] >> r[i];
    vector<long long> pr(m + 1);
    for (int i = 0; i < m; i++) pr[i + 1] = pr[i] + r[i];
    auto f = [&](long long len) -> long long {
        auto i = upper_bound(p.begin(), p.end(), len) - p.begin();
        return len * d + pr[i];
    };

    long long fn = f(n);
    auto ff = [&](long long len) -> long long {
        long long q = len / n, r = len % n;
        return q * fn + f(r);
    };

    vector<long long> idx{0};
    for (int i = 0; i < m; i++) {
        if (p[i] < n) idx.push_back(p[i]);
    }

    for (long long x: idx) {
        for (long long y: idx) {
            if (f(x) + f(y) > ff(x + y + 1)) {
                return void(cout << "YES\n");
            }
        }
    }
    cout << "NO\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

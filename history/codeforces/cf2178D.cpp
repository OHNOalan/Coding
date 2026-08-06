#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int &x: a) {
        cin >> x;
    }
    if (m > n / 2) {
        cout << "-1\n";
        return;
    }

    assert(m <= n / 2);

    vector<int> f(n);
    iota(f.begin(), f.end(), 0);
    ranges::sort(f, {}, [&](int i) -> int { return a[i]; });

    vector<int> h = a;
    vector<int> attack(n);

    auto ops = [&](int i, int j) -> void {
        int x = f[i], y = f[j];
        cout << x + 1 << " " << y + 1 << '\n';
        assert(h[x] > 0 && h[y] > 0 && attack[x] == false);
        tie(h[x], h[y]) = make_pair(h[x] - a[y], h[y] - a[x]);
        attack[x] = true;
    };

    if (m == 0) {
        int j = n - 2, sum = 0;
        while (j >= 0 && sum < a[f[n - 1]]) {
            sum += a[f[j]];
            j--;
        }
        if (sum < a[f[n - 1]]) {
            cout << "-1\n";
            return;
        }
        cout << n - 1 << '\n';
        for (int i = 0; i <= j; i++) {
            ops(i, i + 1);
        }
        for (int i = j + 1; i < n - 1; i++) {
            ops(i, n - 1);
        }
    } else {
        cout << n - (2 * m) + m << '\n';
        for (int i = 0; i < n - (2 * m); i++) {
            ops(i, i + 1);
        }
        for (int i = n - (2 * m); i < n; i += 2) {
            ops(i + 1, i);
        }
    }

    for (int i = 0; i < n; i++) {
        assert(h[i] <= 0 || attack[i]);
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

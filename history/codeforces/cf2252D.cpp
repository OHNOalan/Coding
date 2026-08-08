#include <bits/stdc++.h>
#include <cstdarg>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    n--;
    vector<long long> d(n);
    for (int i = 0; i < n; ++i) d[i] = a[i + 1] - a[i];
    for (int i = 0; i < n;) {
        int j = i + 1;
        while (j < n && (((d[i] ^ d[j]) & 1) == 0)) j++;
        ranges::sort(d.begin() + i, d.begin() + j);
        i = j;
    }
    long long x = a[0];
    cout << x << ' ';
    for (int i = 0; i < n; ++i) {
        x += d[i];
        cout << x << ' ';
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

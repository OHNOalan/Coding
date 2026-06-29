#include <bits/stdc++.h>

using namespace std;

void solve() {
    long long n, a, b;
    cin >> n >> a >> b;
    if (a * 3 > b) {
        cout << (n / 3) * b + min(b, (n % 3) * a) << '\n';
    } else {
        cout << n * a << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

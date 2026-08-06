#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    cout << ((-n % 3 + 3) % 3) << '\n';
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

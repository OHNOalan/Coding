#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    int a = 1, b = 0, step = 0;
    while (a + b <= n && a <= m) {
        tie(a, b) = make_pair(a + b, a);
        step++;
    }
    swap(a, b);
    if (a <= n && b <= m) {
        step++;
        if (a + b <= m && b <= n) {
            tie(a, b) = make_pair(b, a + b);
            step++;
        }
    } else {
        swap(a, b);
    }
    cout << step << ' ' << a << ' ' << b << '\n';
}

#define MULTICASE true

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

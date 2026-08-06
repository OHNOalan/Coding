#include <bits/stdc++.h>
using namespace std;
int init = []() -> int { return 0; }();

void solve() {
    int n, k;
    cin >> n >> k;
    string s;
    cin >> s;
    vector<int> f(2);
    for (char c: s)
        f[c - '0']++;
    if (f[0] > f[1])
        swap(f[0], f[1]);
    int mn = (n >> 1) - f[0];
    if (k < mn || ((k ^ mn) & 1))
        cout << "NO\n";
    else
        cout << "YES\n";
}


#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

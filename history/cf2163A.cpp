#include <bits/stdc++.h>

using namespace std;

#define YES cout << "YES\n"
#define NO cout << "NO\n"

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    sort(a.begin(), a.end());
    for (int i = 1; i < n - 1; i += 2) {
        if (a[i] != a[i + 1]) {
            NO;
            return;
        }
    }
    YES;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

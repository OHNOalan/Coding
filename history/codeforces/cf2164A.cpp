#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    int x;
    cin >> x;
    int mn = *min_element(a.begin(), a.end());
    int mx = *max_element(a.begin(), a.end());
    if (mn <= x && x <= mx)
        cout << "YES\n";
    else
        cout << "NO\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

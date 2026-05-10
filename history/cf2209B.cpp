#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    vector<int> ans(n);
    multiset<int> st;
    for (int i = n - 1; i >= 0; i--) {
        int l = 0, r = 0;
        for (int j = i + 1; j < n; j++) {
            if (a[j] > a[i])
                r++;
            else if (a[j] < a[i])
                l++;
        }
        ans[i] = max(l, r);
    }
    for (int x: ans) cout << x << " ";
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

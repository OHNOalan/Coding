#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    i64 sum = accumulate(a.begin(), a.end(), 0ll);
    int cnt_one = n - count(a.begin(), a.end(), 0);
    i64 sum2 = sum - cnt_one;
    i64 sub = n - 1 - sum2;
    cout << cnt_one - max(0ll, sub) << endl;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

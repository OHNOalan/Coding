#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    array<int, 2> cnt{};
    for (int x: a) cnt[x % 2]++;
    if (cnt[0] > 0 && cnt[1] > 0) {
        sort(a.begin(), a.end());
    }
    for (int x: a) cout << x << " ";
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

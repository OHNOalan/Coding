#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
const int INF = INT_MAX / 2;

void solve() {
    int n;
    cin >> n;
    string s;
    cin >> s;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        if (s[i] == 'a')
            a[i] = 1;
        else
            a[i] = -1;
    }

    int target = 2 * count(s.begin(), s.end(), 'a') - n;
    if (target == 0) {
        cout << "0\n";
        return;
    }

    if (abs(target) == n) {
        cout << "-1\n";
        return;
    }

    vector<int> lst(2 * n + 1, -1);
    lst[n] = 0;
    int cur = 0, ans = INF;
    for (int i = 0; i < n; i++) {
        cur += a[i];
        if (lst[n + cur - target] >= 0) {
            ans = min(ans, i - lst[n + cur - target] + 1);
        }
        lst[n + cur] = i + 1;
    }
    cout << (ans < n ? ans : -1) << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

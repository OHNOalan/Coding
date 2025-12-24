#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    i64 k;
    cin >> n >> k;
    vector<int> q(n), r(n);
    for (int &x: q) cin >> x;
    for (int &x: r) cin >> x;
    sort(q.begin(), q.end());
    sort(r.begin(), r.end());
    int left = 0, right = n + 1;
    auto check = [&](int mid) -> bool {
        for (int i = 0; i < mid; i++) {
            if ((i64) (q[i] + 1) * (r[mid - 1 - i] + 1) - 1 > k) {
                return false;
            }
        }
        return true;
    };
    while (left + 1 < right) {
        int mid = (left + right) / 2;
        (check(mid) ? left : right) = mid;
    }
    cout << left << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

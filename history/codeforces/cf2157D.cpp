#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve1() {
    i64 n, l, r;
    cin >> n >> l >> r;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    sort(a.begin(), a.end());
    vector<i64> A(n + 1);
    for (int i = 0; i < n; i++) A[i + 1] = A[i] + a[i];
    i64 ans = 0;
    for (int i = 0; i < n; i++) {
        ans = max(ans, min(A[n] - A[i] - A[i] + (i - (n - i)) * r, A[n] - A[i] - A[i] + (i - (n - i)) * l));
        ans = max(ans,
                  min(A[n] - A[i + 1] - A[i] + (i - (n - i - 1)) * l, A[n] - A[i + 1] - A[i] + (i - (n - i - 1)) * r));
    }
    ans = max(ans, min(n * l - A[n], n * r - A[n]));
    cout << ans << '\n';
}

void solve() {
    i64 n, l, r;
    cin >> n >> l >> r;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    i64 diff = 0;
    for (int &x: a) {
        if (x < l) {
            diff += l - x;
            x = l;
        } else if (x > r) {
            diff += x - r;
            x = r;
        }
    }
    sort(a.begin(), a.end());
    vector<i64> A(n + 1);
    for (int i = 0; i < n; i++) A[i + 1] = A[i] + a[i];
    i64 ans = A[n] - A[n / 2] - A[(n + 1) / 2];
    cout << ans + diff << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

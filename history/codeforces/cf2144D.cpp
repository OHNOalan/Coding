#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int MOD = 998244353;

void solve() {
    int n, y;
    cin >> n >> y;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    i64 A = *max_element(a.begin(), a.end()) + 1;

    vector<int> cnt(A);
    for (int x: a) cnt[x]++;
    vector<int> pre(A);
    for (int i = 1; i < A; i++) {
        pre[i] = pre[i - 1] + cnt[i];
    }


    i64 ans = LLONG_MIN;
    if (A == 2) {
        cout << n << '\n';
        return;
    }

    for (i64 i = 2; i < A; i++) {
        i64 tot = 0;

        for (int p = 1; p < A; p++) {
            i64 l = i * (p - 1);
            i64 r = min(A - 1, i * p);
            if (l >= A) break;

            int f = pre[r] - pre[l];
            tot += i64(f) * p;
            tot -= i64(y) * max(0, f - cnt[p]);
        }
        ans = max(ans, tot);
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

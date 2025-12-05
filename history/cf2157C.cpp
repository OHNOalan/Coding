#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, k, q;
    cin >> n >> k >> q;
    vector<int8_t> mn(n), mx(n);
    for (int i = 0; i < q; i++) {
        int c, l, r;
        cin >> c >> l >> r;
        l--, r--;
        if (c == 1) {
            for (int i = l; i <= r; i++) {
                mn[i] = true;
            }
        } else {
            for (int i = l; i <= r; i++) {
                mx[i] = true;
            }
        }
    }
    int cur = 0;
    vector<int> ans(n);
    for (int i = 0; i < n; i++) {
        if (!mn[i] && !mx[i])
            ans[i] = 0;
        else if (mn[i] && mx[i])
            ans[i] = k + 1;
        else if (mn[i] && !mx[i])
            ans[i] = k;
        else {
            ans[i] = cur;
            cur = (cur + 1) % k;
        }
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

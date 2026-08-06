#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, k, x;
    cin >> n >> k >> x;
    vector<int> a(n);
    for (int &b: a) cin >> b;
    sort(a.begin(), a.end());

    vector<int> ans;

    auto check = [&](int m) -> bool {
        ans.clear();
        int kk = k;
        int lst = -m;
        for (int b: a) {
            for (int i = b - m; kk > 0 && i - lst >= m; i--) {
                ans.push_back(i);
                kk--;
            }
            lst = b;
            if (kk == 0)
                break;
        }
        for (int i = lst + m; kk > 0 && i <= x; i++) {
            ans.push_back(i);
            kk--;
        }
        return kk == 0;
    };

    int l = 0, r = x + 1;
    while (l + 1 < r) {
        int m = (l + r) / 2;
        (check(m) ? l : r) = m;
    }
    if (l == 0) {
        ans.resize(k);
        iota(ans.begin(), ans.end(), 0);
    } else {
        check(l);
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

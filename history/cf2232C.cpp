#include <bits/stdc++.h>

using namespace std;
using ll = long long;

void solve() {
    int n, x;
    ll s;
    cin >> n >> x >> s;

    string S;
    cin >> S;

    auto f = [&](int m) -> ll {
        ll ans = 0, T = 0;
        for (char c: S) {
            if (c == 'I') {
                if (T < x) {
                    ++T;
                    ++ans;
                }
            } else if (c == 'E') {
                if (ans < T * s) {
                    ++ans;
                }
            } else {
                assert(c == 'A');
                if (m-- > 0) {
                    if (T < x) {
                        ++T;
                        ++ans;
                    }
                } else {
                    if (ans < T * s) {
                        ++ans;
                    }
                }
            }
        }
        return ans;
    };

    int l = -1, r = 1;
    for (auto c: S)
        if (c == 'A') ++r;
    while (l + 1 < r) {
        int m = (l + r) / 2;
        ll fm = f(m), fm1 = f(m + 1);
        (fm < fm1 ? l : r) = m;
    }
    cout << f(r) << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

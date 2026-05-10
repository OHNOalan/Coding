#include <bits/stdc++.h>

using namespace std;

void solve() {
    vector<pair<int, char>> cs{{0, 'R'}, {0, 'G'}, {0, 'B'}};
    cin >> cs[0].first >> cs[1].first >> cs[2].first;
    sort(cs.begin(), cs.end());

    auto [n1, c1] = cs[0];
    auto [n2, c2] = cs[1];
    auto [n3, c3] = cs[2];

    n3 = min(n3, n1 + n2 + 1);
    cs[2].first = n3;

    if (n2 == n3 && n1 <= 2) {
        string s;
        if (n1 >= 1) s += c1;
        for (int i = 1; i <= n2; i++) {
            s += c2;
            s += c3;
        }
        if (n1 >= 2) s += c1;
        cout << s << '\n';
        return;
    }

    int sz = n1 + n2 + n3;
    string s(sz, '_');
    reverse(cs.begin(), cs.end());
    int j = 0;
    for (auto [num, c]: cs) {
        for (int i = 0; i < num; i++) {
            s[j] = c;
            j += 2;
            if (j >= sz) j = 1;
        }
    }
    cout << s << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

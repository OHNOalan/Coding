#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;
    vector<int> ops;
    for (int i = 0; i < n; i++) {
        if (s[i] == 'A') {
            if (ops.empty() || ops.back() == -1)
                ops.push_back(0);
            ops.back()++;
        } else {
            ops.push_back(-1);
        }
    }
    int B = count(s.begin(), s.end(), 'B');
    while (q--) {
        int x;
        cin >> x;
        if (B == 0) {
            cout << x << '\n';
            continue;
        }
        int cur = 0, i = 0;
        int time = 0;
        while (x > 0) {
            if (ops[i] > 0) {
                int mn = min(ops[i], x);
                time += mn;
                x -= mn;
            } else {
                x /= 2;
                time++;
            }
            i++;
            if (i == ops.size())
                i = 0;
        }
        cout << time << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

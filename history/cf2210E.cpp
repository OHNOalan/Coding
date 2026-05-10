#include <bits/stdc++.h>

using namespace std;

map<pair<int, int>, int> queries;
int ask(int l, int r) {
    if (l > r) return 0;
    if (queries.count({l, r})) {
        return queries[{l, r}];
    }
    cout << "? " << l << " " << r << endl;
    int x;
    cin >> x;
    return queries[{l, r}] = ((r - l + 1) / x) % 2;
}

int guess(string s) {
    cout << "! " << s << endl;
    int res;
    cin >> res;
    return res;
}

void solve() {
    int n;
    cin >> n;

    string s(n, '0');
    queries.clear();
    int cnt = 0;
    for (int i = 2; i < n; i++) {
        int quer1 = i - i % 2;
        int quer2 = (n - i) - (n - i) % 2;
        if (quer1 > quer2) {
            int type1 = ask(1 + (i) % 2, i);
            int type2 = ask(1 + (i) % 2, i - 2);
            s[i - 1] = ((s[i - 2] - '0') ^ type1 ^ type2) + '0';
        } else {
            int type1 = ask(i + 1, n - (n - i) % 2);
            int type2 = ask(i - 1, n - (n - i) % 2);
            s[i - 1] = ((s[i - 2] - '0') ^ type1 ^ type2) + '0';
        }
    }
    int la = ask(1 + (n & 1), n);
    int ct = 0;
    for (int i = (n & 1); i < n; i++) {
        ct += (s[i] == '1');
    }
    ct &= 1;
    s[n - 1] = (la ^ ct) + '0';
    if (guess(s)) return;
    for (auto &x: s) {
        x ^= 1;
    }
    assert(guess(s));
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

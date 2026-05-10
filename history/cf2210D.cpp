#include <bits/stdc++.h>

using namespace std;

#define YES cout << "YES\n"
#define NO cout << "NO\n"

void solve() {
    int n;
    cin >> n;
    string s, t;
    cin >> s >> t;
    auto f = [&](string &s) -> int {
        int cnt = 0;
        for (int i = 0; i < n - 1; i++) {
            if (s[i] == '(' and s[i + 1] == ')') cnt++;
        }
        return cnt;
    };
    auto g = [&](string &s) -> int {
        vector<int> cnt(n);
        int depth = 0;
        for (int i = 0; i < n; i++) {
            if (s[i] == '(') {
                depth++;
            } else {
                cnt[--depth]++;
            }
        }
        for (int i = 0; i < n; i++) {
            if (cnt[i] != 1) return i;
        }
        __builtin_unreachable();
    };

    if (f(s) == f(t) && g(s) == g(t))
        YES;
    else
        NO;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

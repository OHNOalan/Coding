#include <bits/stdc++.h>

using namespace std;

int n, m, x, ans;
char c;

int main() {
    // if DEBUG=1 then redirect cin from stdin to $1
    if (getenv("DEBUG") != nullptr) {
        freopen(getenv("DEBUG"), "r", stdin);
    }
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m;
    vector<int> a(n);
    multiset<int> st;
    for (int i = 0; i < n; i++) {
        cin >> x;
        st.insert(x);
    }
    int pre = 0;
    for (int i = 0; i < m; i++) {
        cin >> c >> x;
        // x ^= pre;
        if (c == 'I') {
            st.insert(x);
        } else if (c == 'R') {
            auto it = st.find(x);
            if (it != st.end())
                st.erase(it);
        } else if (c == 'S') {
            if (x >= 1 && x <= st.size()) {
                int pos = 1;
                for (int v: st) {
                    if (x == pos) {
                        ans = v;
                        break;
                    }
                    pos++;
                }
            } else {
                ans = -1;
            }
            cout << ans << '\n';
            pre = ans;
        } else if (c == 'L') {
            auto it = st.lower_bound(x);
            if (it != st.end() && *it == x) {
                int pos = 1;
                for (int v: st) {
                    if (x == v) {
                        ans = pos;
                        break;
                    }
                    pos++;
                }
            } else {
                ans = -1;
            }
            cout << ans << '\n';
            pre = ans;
        }
    }
    for (int x: st) cout << x << " ";
    cout << endl;
    return 0;
}

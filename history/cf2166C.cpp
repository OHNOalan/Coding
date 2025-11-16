#include <bits/stdc++.h>

using namespace std;
#define debug(arr)                                                                                                     \
    cerr << #arr << ": ";                                                                                              \
    for (const auto &_x: arr) {                                                                                        \
        cerr << _x << " ";                                                                                             \
    }                                                                                                                  \
    cerr << endl;


void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    vector<int> b(2 * n);
    for (int i = 0; i < n; i++) {
        b[i] = b[i + n] = a[i];
    }
    stack<int> st;
    vector<int> l(2 * n, INT_MAX), r(2 * n, INT_MAX);
    for (int i = 0; i < 2 * n; i++) {
        while (!st.empty() && b[st.top()] < b[i]) {
            r[st.top()] = b[i];
            st.pop();
        }
        if (!st.empty()) {
            l[i] = b[st.top()];
        }
        st.push(i);
    }

    vector<int> mn(n);
    for (int i = 0; i < n; i++) {
        mn[i] = min({l[i], l[i + n], r[i], r[i + n]});
    }

    cout << accumulate(mn.begin(), mn.end(), 0ll) - *max_element(mn.begin(), mn.end()) << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

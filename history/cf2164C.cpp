#include <bits/stdc++.h>

using namespace std;

void solve1() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m), c(m);
    for (int &x: a) cin >> x;
    for (int &x: b) cin >> x;
    for (int &x: c) cin >> x;

    vector<int> idx(m);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](const int i, const int j) -> bool {
        if (c[i] == 0 && c[j] == 0) {
            return b[i] < b[j];
        } else if (c[i] == 0) {
            return false;
        } else if (c[j] == 0) {
            return true;
        } else {
            return b[i] < b[j] || b[i] == b[j] && c[i] < c[j]; // b[i] < b[j] might be good enough
        }
    });

    priority_queue<int, vector<int>, greater<>> pq;
    for (int x: a) pq.push(x);
    vector<int> left;

    int cur = 0, ans = 0;
    while (cur < m && !pq.empty()) {
        int i = idx[cur];
        if (c[i] == 0) break;
        int x = pq.top();
        pq.pop();
        if (x >= b[i]) {
            ans++;
            pq.push(max(x, c[i]));
            cur++;
        } else {
            left.push_back(x);
        }
    }
    for (int x: left) pq.push(x);
    while (cur < m && c[idx[cur]] != 0) cur++;
    while (cur < m && !pq.empty()) {
        int i = idx[cur];
        int x = pq.top();
        pq.pop();
        if (x >= b[i]) {
            cur++;
            ans++;
        }
    }
    cout << ans << '\n';
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> A(n), B(m), C(m);
    for (int &x: A) cin >> x;
    for (int &x: B) cin >> x;
    for (int &x: C) cin >> x;

    vector<pair<int, int>> r1, r2;
    for (int i = 0; i < m; i++) {
        (C[i] == 0 ? r2 : r1).push_back({B[i], C[i]});
    }

    int ans = 0;

    multiset<int> st(A.begin(), A.end());
    sort(r1.begin(), r1.end());
    for (auto [b, c]: r1) {
        auto it = st.lower_bound(b);
        if (it != st.end()) {
            int a = *it;
            st.erase(it);
            st.insert(max(a, c));
            ans++;
        }
    }
    sort(r2.begin(), r2.end());
    for (auto [b, c]: r2) {
        auto it = st.lower_bound(b);
        if (it != st.end()) {
            st.erase(it);
            ans++;
        }
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

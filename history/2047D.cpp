#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    stack<int> st;
    priority_queue<int, vector<int>, greater<>> pq;
    for(int i=0; i<n; i++) {
        int x = a[i];
        while(st.size() && st.top() > x) {
            pq.push(st.top() + 1);
            st.pop();
        }
        st.push(x);
    }
    while(st.size() && pq.size() && st.top() > pq.top()) {
        pq.push(st.top() + 1);
        st.pop();
    }
    while(pq.size()) {
        st.push(pq.top());
        pq.pop();
    }
    assert(st.size() == n);
    vector<int> ans(n);
    for(int i=0; i<n; i++) {
        ans[i] = st.top(); st.pop();
    }
    ranges::reverse(ans);
    for(int i=0; i<n; i++) {
        cout << ans[i] << " ";
    }
    cout << endl;
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

#include <bits/stdc++.h>

using namespace std;

int hb(int x) {
    x >>= 1;
    int ret = 1;
    while (x) {
        x >>= 1;
        ret <<= 1;
    }
    return ret;
}

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    sort(a.rbegin(), a.rend());
    while (q--) {
        int c;
        cin >> c;

        int ans = 0;

        priority_queue<int> pq;
        int i = 0;
        while (c) {
            int x = 0;
            if (i == n || (pq.size() && pq.top() > a[i])) {
                x = pq.top();
                pq.pop();
            } else {
                x = a[i++];
            }
            x -= hb(c);
            ans += max(0, -x);
            x = max(x, 0);
            pq.push(x);
            c -= hb(c);
        }
        cout << ans << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

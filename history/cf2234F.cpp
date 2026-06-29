#include <bits/stdc++.h>

using namespace std;

using pii = pair<long long, long long>;

void solve() {
    long long n;
    cin >> n;
    vector<long long> h(n);
    for (long long i = 0; i < n; i++) cin >> h[i];
    long long t = max_element(h.begin(), h.end()) - h.begin();

    vector<long long> ls(n), rs(n);

    stack<pii> sm;
    sm.push({1e18, 0});
    for (long long ti = 1; ti < n; ti++) {
        long long i = (ti + t) % n;
        long long s = ls[i] + h[i], c = 1;
        while (sm.top().first <= h[i]) {
            s += sm.top().second * (h[i] - sm.top().first);
            c += sm.top().second;
            sm.pop();
        }
        sm.push({h[i], c});
        ls[(i + 1) % n] = s;
    }

    sm = stack<pii>();
    sm.push({1e18, 0});
    for (long long ti = 1; ti < n; ti++) {
        long long i = (t + n - ti) % n;
        long long s = rs[(i + 1) % n] + h[i], c = 1;
        while (sm.top().first <= h[i]) {
            s += sm.top().second * (h[i] - sm.top().first);
            c += sm.top().second;
            sm.pop();
        }
        sm.push({h[i], c});
        rs[i] = s;
    }

    for (long long i = 0; i < n; i++) {
        cout << ls[i] + rs[i] << " ";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    long long t;
    cin >> t;
    while (t--) {
        solve();
    }
}

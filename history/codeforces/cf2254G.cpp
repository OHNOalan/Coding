#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    vector<vector<int>> g(n);
    for (int x = 1, p; x < n; ++x) {
        cin >> p, --p;
        g[p].push_back(x);
    }
    auto s = [&](this auto &&self, int u) -> multiset<int> {
        multiset<int> s{a[u]};
        for (int v: g[u]) {
            auto ns = self(v);
            if (ns.size() > s.size()) swap(s, ns);
            for (int x: ns) s.insert(x);
        }
        if (!g[u].empty()) s.erase(s.begin());
        return s;
    }(0);
    int c = s.size();
    for (int i = 1; i < c; ++i) cout << "-1 ";
    multiset<int> t(a.begin(), a.end());
    for (int x: s) t.erase(t.find(x));
    long long sum = 0;
    for (int x: s) sum += x;
    cout << sum << ' ';
    for (auto it = t.rbegin(); it != t.rend(); ++it) {
        sum += *it;
        cout << sum << ' ';
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

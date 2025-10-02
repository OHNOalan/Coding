#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<ll, ll>;
const ll INF = LLONG_MAX / 2;

void solve() {
    ll n;
    cin >> n;
    vector<ll> a(n), b(n);
    for (ll i = 0; i < n; i++)
        cin >> a[i];

    for (ll i = 0; i < n; i++) {
        cin >> b[i];
        b[i]--;
    }

    vector<vector<array<ll, 2>>> g(n);
    for (ll i = 0; i < n; i++) {
        if (i)
            g[i].push_back({i - 1, 0});
        if (b[i] > i)
            g[i].push_back({b[i], a[i]});
    }

    vector<ll> dist(n, INF);
    dist[0] = 0;
    priority_queue<pii, vector<pii>, greater<>> pq;
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [dx, x] = pq.top();
        pq.pop();
        for (auto [y, w] : g[x]) {
            ll n_dis = dx + w;
            if (n_dis < dist[y]) {
                dist[y] = n_dis;
                pq.emplace(n_dis, y);
            }
        }
    }

    ll ans = 0;
    for (ll i = 0, sum = 0; i < n; i++) {
        sum += a[i];
        ans = max(ans, sum - dist[i]);
    }
    cout << ans << '\n';
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}
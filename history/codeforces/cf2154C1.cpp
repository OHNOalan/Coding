#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
constexpr int N = 2e5 + 10;

vector<vector<int>> pfac(N + 1);

void init() {
    for (int i = 2; i <= N; i++) {
        if (pfac[i].empty()) {
            for (int j = i; j <= N; j += i) { pfac[j].push_back(i); }
        }
    }
}


void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    vector<i64> b(n);
    for (i64 &x: b) cin >> x;

    int ans = 2;
    map<int, int> cnt;
    for (int i = 0; i < n; i++) {
        for (int x: pfac[a[i]]) {
            if (cnt[x] > 0)
                ans = 0;
            cnt[x]++;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int x: pfac[a[i]]) cnt[x]--;

        for (int x: pfac[a[i] + 1]) {
            if (cnt[x] > 0)
                ans = min(ans, 1);
        }

        for (int x: pfac[a[i]]) cnt[x]++;
    }

    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();

    int t;
    cin >> t;
    while (t--) solve();
}

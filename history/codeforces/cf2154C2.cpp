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
    vector<int> b(n);
    for (int &x: b) cin >> x;

    int mi = 0, mn = INT_MAX / 2, mn2 = INT_MAX / 2;
    for (int i = 0; i < n; i++)
        if (b[i] < mn) {
            mn2 = mn;
            mn = b[i];
            mi = i;
        } else if (b[i] < mn2) {
            mn2 = b[i];
        }
    i64 ans = mn + mn2;
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
                ans = min(ans, (i64) b[i]);
        }

        for (int x: pfac[a[i]]) cnt[x]++;
    }

    set<int> check;
    for (int i = 0; i < n; i++)
        if (i != mi) {
            for (int x: pfac[a[i]]) check.insert(x);
        }
    for (int x: check) {
        int times = x - (a[mi] % x);
        if (times == x)
            times = 0;

        ans = min(1LL * ans, 1LL * times * mn);
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

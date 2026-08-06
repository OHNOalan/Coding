#include <bits/stdc++.h>

using namespace std;

void solve1() {
    int n;
    cin >> n;
    vector a(2, vector<int>(n));
    for (auto &b: a) {
        for (int &x: b) {
            cin >> x;
        }
    }

    long long ans = 0;
    for (int l = 1; l <= a[0][0]; l++) {
        int left = max(l, a[0][0]) - 1, right = 2 * n + 1;
        while (left + 1 < right) {
            int r = (left + right) / 2;
            vector<int> work(n);
            work[0] = true;
            for (int i = 1; i < n; i++) {
                work[i] = work[i - 1] && (l <= a[0][i] && a[0][i] <= r);
            }
            work[0] = work[0] && (l <= a[1][0] && a[1][0] <= r);
            for (int i = 1; i < n; i++) {
                work[i] = (work[i] || work[i - 1]) && (l <= a[1][i] && a[1][i] <= r);
            }
            (work[n - 1] ? right : left) = r;
        }
        ans += 2 * n - left;
    }
    cout << ans << '\n';
}


void solve2() {
    int n;
    cin >> n;
    vector a(3, vector<int>(n + 1));
    for (int i = 1; i < 3; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> a[i][j];
        }
    }

    long long ans = 0;
    for (int l = 1, r = 1; l <= 2 * n; l++) {
        if (r < l) r++;
        assert(l <= r);
        while (r <= 2 * n) {
            vector<int> work(n + 1);
            work[0] = 1;
            for (int i = 1; i < 3; i++) {
                for (int j = 1; j <= n; j++) {
                    work[j] = (work[j] || work[j - 1]) && (l <= a[i][j] && a[i][j] <= r);
                }
            }
            if (work[n]) break;
            r++;
        }
        ans += 2 * n - r + 1;
    }
    cout << ans << '\n';
}

const int INF = 1e9;

void solve3() {
    int n;
    cin >> n;
    vector a(2, vector<int>(n));
    for (auto &b: a) {
        for (int &x: b) {
            cin >> x;
        }
    }

    vector<vector<pair<int, int>>> pos(2 * n + 1);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < n; j++) {
            pos[a[i][j]].push_back({i, j});
        }
    }


    array<set<int>, 2> st;
    st[0].insert(INF);
    st[1].insert(-INF);
    for (int i = 0; i < n; i++) st[0].insert(i);
    for (int i = 0; i < n; i++) st[1].insert(i);

    auto add = [&](int x) {
        for (auto [i, j]: pos[x]) {
            st[i].erase(j);
        }
    };

    auto del = [&](int x) {
        for (auto [i, j]: pos[x]) {
            st[i].insert(j);
        }
    };


    auto check = [&]() {
        if (st[0].count(0)) return false;
        if (st[1].count(n - 1)) return false;
        if (*st[0].begin() - 1 <= *st[1].rbegin()) return false;

        return true;
    };


    long long ans = 0;
    for (int l = 1, r = 1; l <= 2 * n; l++) {
        while (r <= 2 * n && !check()) {
            add(r++);
        }
        if (!check()) break;

        ans += 2 * n - r + 2;
        del(l);
    }
    cout << ans << '\n';
}


void solve() {
    int n;
    cin >> n;
    vector a(2, vector<int>(n));
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
            a[i][j]--;
        }
    auto pmn = a[0], pmx = a[0], smn = a[1], smx = a[1];
    for (int i = 1; i < n; i++) {
        pmn[i] = min(pmn[i], pmn[i - 1]);
        pmx[i] = max(pmx[i], pmx[i - 1]);
    }
    for (int i = n - 2; i >= 0; i--) {
        smn[i] = min(smn[i], smn[i + 1]);
        smx[i] = max(smx[i], smx[i + 1]);
    }

    vector<int> f(2 * n, 2 * n);
    for (int i = 0; i < n; i++) {
        int L = min(pmn[i], smn[i]);
        int R = max(pmx[i], smx[i]);
        f[L] = min(f[L], R);
    }

    long long ans = 2 * n - f[2 * n - 1];
    for (int i = 2 * n - 2; i >= 0; i--) {
        f[i] = min(f[i], f[i + 1]);
        ans += 2 * n - f[i];
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

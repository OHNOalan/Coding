#include <bits/stdc++.h>

using namespace std;
const int MOD = 1000000007;

void solve() {
    int n, k;
    cin >> n >> k;
    vector vis(n, vector<int>(n, -1));
    for (int i = 0; i < k; ++i) {
        int s, r, c;
        cin >> s >> r >> c;
        --r, --c;
        string S;
        if (s != 1) cin >> S;

        int id = n - s / 2 - 1;
        for (int j = 0; j < s / 2; ++j) {
            vis[r][c] = id;
            ++(S[j] == 'D' ? r : c);
        }
        vis[r][c] = id;
    }

    long long ans = 1;
    vector<int> v1, v2;
    v1.reserve(n);
    v2.reserve(n);
    for (int i = 0; i < n; ++i) {
        bool assigned = false;
        for (int j = 0; j <= i; ++j) {
            if (vis[j][i - j] == i) {
                assigned = true;
                break;
            }
        }
        if (assigned) continue;
        int c = 0;
        for (int j = 0; j <= i - 1; ++j) {
            if (vis[j][i - 1 - j] == -1) {
                ++c;
            } else {
                v1.push_back(c);
                c = 0;
            }
        }
        v1.push_back(c);
        c = 0;

        for (int j = 0; j <= i; ++j) {
            if (vis[j][i - j] == -1) {
                ++c;
            } else {
                v2.push_back(c);
                c = 0;
            }
        }
        v2.push_back(c);

        for (int j = 0; j < v1.size(); j++) {
            if (v1[j] != v2[j]) ans = ans * v2[j] % MOD;
        }
        v1.clear();
        v2.clear();
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

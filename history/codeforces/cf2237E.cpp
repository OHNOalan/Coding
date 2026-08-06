#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i], a[i]--;
    }
    for (int i = 0; i < n; i++) {
        cin >> b[i];
        if (b[i] != -1) b[i]--;
    }
    vector<int> vis(n);
    for (int i = 0; i < n; i++) {
        if (b[i] != -1 and !vis[i]) {
            int j = i;
            while (!vis[j]) {
                vis[j] = 1;
                if (b[a[j]] == -1) {
                    b[a[j]] = a[b[j]];
                } else if (b[a[j]] != a[b[j]]) {
                    return void(cout << "NO\n");
                }
                j = a[j];
            }
        }
    }

    vector<int> cnt(n);
    for (int i = 0; i < n; ++i) {
        if (b[i] == -1) continue;
        cnt[b[i]]++;
    }
    for (int i = 0; i < n; ++i) {
        if (cnt[i] > 1) {
            cout << "NO\n";
            return;
        }
    }

    vector<int> vis1(n), vis2(n);
    vector<vector<int>> cycle1(n + 1), cycle2(n + 1);
    for (int i = 0; i < n; ++i) {
        if (b[i] == -1 and !vis1[i]) {
            int j = i, len = 0;
            while (!vis1[j]) {
                vis1[j] = 1;
                len++;
                j = a[j];
            }
            cycle1[len].push_back(i);
        }
    }
    for (int i = 0; i < n; ++i) {
        if (cnt[i] == 0 and !vis2[i]) {
            int j = i, len = 0;
            while (!vis2[j]) {
                vis2[j] = 1;
                len++;
                j = a[j];
            }
            cycle2[len].push_back(i);
        }
    }
    for (int len = 1; len <= n; ++len) {
        for (int i = 0; i < cycle1[len].size(); ++i) {
            int x = cycle1[len][i], y = cycle2[len][i];
            while (b[x] == -1) {
                b[x] = y;
                x = a[x];
                y = a[y];
            }
        }
    }
    cout << "YES\n";
    for (int i = 0; i < n; ++i) {
        cout << b[i] + 1 << " \n"[i == n - 1];
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
using li = long long;

const int N = 10;
const int LOG = 50;
const int LCM = 2520;

int n;
i64 m;
int a[N], b[N];
i64 go[LOG][N][LCM];

void solve() {
    cin >> n >> m;
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < LCM; ++y) { go[0][x][y] = ((y + 1) % a[x]) != b[x]; }
    }
    for (int i = 1; i < LOG; ++i) {
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < LCM; ++y) {
                go[i][x][y] = go[i - 1][x][y] + go[i - 1][(x + (1LL << (i - 1))) % n][(y + go[i - 1][x][y]) % LCM];
            }
        }
    }
    if (go[LOG - 1][0][0] < m) {
        cout << -1 << endl;
        return;
    }
    li s = 0, ans = 0;
    for (int i = LOG - 1; i >= 0; --i) {
        if (s + go[i][ans % n][s % LCM] < m) {
            s += go[i][ans % n][s % LCM];
            ans += 1LL << i;
        }
    }
    cout << ans + 1 << endl;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

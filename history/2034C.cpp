#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

string DIR("UDRL");
constexpr int dirs[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
map<char, int> dirs_map;

void init() {
    for(int i=0; i<DIR.size(); i++) {
        dirs_map[DIR[i]] = i;
    }
}

void solve() {
    int n, m; cin >> n >> m;
    vector<string> G(n);
    for (int i = 0; i < n; i++) cin >> G[i];
    // -1 not visit, 0 bad, 1 good, 2 travelled;
    vector<vector<int>> f(n, vector<int>(m, -1));
    auto dfs = [&](auto&& dfs, int i, int j) -> bool {
        if(i < 0 || j < 0 || i >= n || j >= m) return false;
        int &res = f[i][j];
        if(res != -1) {
            if(res == 2) {
                res = 1;
            }
            return res;
        }
        char c = G[i][j];
        if(c == '?') {
            for(int d=0; d<4; d++) {
                res = 2;
                if(dfs(dfs, i+dirs[d][0], j+dirs[d][1])) {
                    res = 1;
                    break;
                }
                res = -1;
            }
            if(res == -1) res = 0;
        } else {
            int d = dirs_map[c];
            res = 2;
            if(dfs(dfs, i+dirs[d][0], j+dirs[d][1])) {
                res = 1;
            } else {
                res = 0;
            }
        }
        return res;
    };
    int ans = 0;
    for(int i=0; i<n; i++) for(int j=0; j<m; j++) ans += dfs(dfs, i, j);
    // for(int i=0; i<n; i++) {
    //     for(int j=0; j<m; j++) {
    //         cout << f[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    cout << ans << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

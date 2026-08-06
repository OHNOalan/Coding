#include <bits/stdc++.h>
using namespace std;

using ll = long long;
void solve() {
    int n; cin >> n;
    vector<vector<int>> G(n);
    for(int i=0; i<n-1; i++) {
        int x,y; cin >> x >> y;
        x--, y--;
        G[x].push_back(y);
        G[y].push_back(x);
    }
    array<int, 2> cnt{1, 0};
    vector<int> vis(n, false);
    vis[0] = true;
    vector<int> q{0};
    int level = 1;
    while(q.size()) {
        vector<int> nq;
        for(int x : q) {
            for(int y : G[x]) {
                if(!vis[y]) {
                    vis[y] = true;
                    nq.push_back(y);
                    cnt[level]++;
                }
            }
        }
        level = 1 - level;
        q = move(nq);
    }
    cout << (ll) cnt[0] * cnt[1] - n + 1 << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

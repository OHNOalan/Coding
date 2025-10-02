#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 1e7 + 10;

using ll = long long;

void solve() {
    int n, k; cin >> n >> k;
    vector<int> p(n);
    for (int i = 0; i < n; i++) cin >> p[i];
    int u=0, v=1;
    for(int u=0; u<n-1; u++) {
        for(int v=u+1; v<n; v++) {
            cout << p[u] << " " << p[v] << endl;
            if(--k == 0) return;
        }
    }
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

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    vector<int> b(n+1);
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        while(x) {
            if(x <= n) {
                b[x]++;
            }
            x >>= 1;
        }
    }
    for(int i=n; i>0; i--) {
        if(b[i] <= 0) {
            cout << "NO\n";
            return;
        }
        int x = i;
        while(x) {
            b[x]--;
            x >>= 1;
        }
    }
    cout << "YES\n";
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

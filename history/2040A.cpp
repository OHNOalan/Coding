#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n, k; cin >> n >> k;
    vector<int> b(k);
    map<int, int> mp;
    for(int i=0, x; i<n; i++) {
        cin >> x;
        x %= k;
        b[x]++;
        mp[x] = i+1;
    }
    for(int i=0; i<k; i++) {
        if(b[i] == 1) {
            cout << "YES\n";
            cout << mp[i] << '\n';
            return;
        }
    }
    cout << "NO\n";
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

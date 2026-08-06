#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
static constexpr int MOD = 1e9 + 7;

void solve() {
    int n;
    cin >> n;
    if(n & 1) {
        if(n < 5) {
            cout << "-1\n";
        } else {
            cout << string(n-5, '3') + "36366\n";
        }
    } else {
        cout << string(n-2, '3') + "66\n";
    }
    for(int i=0; i<n; i++) {}
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
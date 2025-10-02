#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
static constexpr int MOD = 1e9 + 7;

void solve() {
    int n;
    cin >> n;
    vector<int> a;
    a.reserve(n);
    if(n & 1) {
        cout << n << '\n';
        for(int i = 2; i<n-1; i+=2) {
            a.push_back(i);
        }
        for(int i = 1; i<n; i+=2) {
            a.push_back(i);
        }
        a.push_back(n-1);
        a.push_back(n);
    } else {
        int k = (sizeof(n) * 8) - __builtin_clz(n) - 1;
        cout << (1<<(k+1))-1 << '\n';
        if(n == (1 << k)) {
            for(int i = 2; i<n-2; i+=2) {
                a.push_back(i);
            }
            for(int i = 1; i<n-1; i+=2) {
                a.push_back(i);
            }
            a.push_back(n-2);
            a.push_back(n-1);
            a.push_back(n);
        } else {
            for(int i=1; i<(1<<k)-1; i++) {
                a.push_back(i);
            }
            for(int i=(1<<k); i<n-1; i++) {
                a.push_back(i);
            }
            a.push_back(n);
            a.push_back(n-1);
            a.push_back((1<<k)-1);
        }
    }
    for(int x : a) {
        cout << x << " ";
    }
    cout << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
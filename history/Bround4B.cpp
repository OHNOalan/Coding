#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2010;
using ll = long long;

void solve() {
    int n; cin >> n;
    vector<int> a(n * 2);
    for(int i = 0; i < n * 2; i++) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    for(int i=0; i<2*n; i+=2) {
        cout << a[i] << ' ';
    }
    for(int i=2*n-1; i>=0; i-=2) {
        cout << a[i] << ' ';
    }
    cout << '\n';
}

#define MULTICASE true
int main() {
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
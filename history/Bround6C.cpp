#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    int ans = 0;
    for(int k=2; k<n; k++) {
        int target = a[k];
        for(int i=0, j=k-1; i<j; i++, j--) {
            ll prod = a[i] * a[j];
            while(i<k-2 && a[i] == a[i+1]) i++;
            while(j>1 && a[j-1] == a[j]) i--;
            if(prod > target) ans++;
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

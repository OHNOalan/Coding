#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2010;
using ll = long long;
void solve() {
    int n, k; cin >> n >> k;
    vector<int> b(n);
    for(int i = 0; i < n; i++)
        cin >> b[i];

    ll total_sum = 0;

    for(int l = 0; l < n; l++) {
        vector<bool> c(MAXN);
        int mex_k = k;
        for(int r = l; r < n; r++) {
            if(!c[b[r]]) {
                c[b[r]] = true;
                if(b[r] <= mex_k) {
                    mex_k++;
                    while(mex_k < MAXN && c[mex_k]) {
                        mex_k++;
                    }
                }
            }
            total_sum += mex_k;
        }
    }

    cout << total_sum << '\n';
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
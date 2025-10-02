#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 1e5;

using ll = long long;

int comp[MAX + 1], fac[MAX + 1];

void init() {
    memset(comp, 0, sizeof(comp));
    memset(fac, 0, sizeof(fac));
    for(int i=2; i<=MAX; i++) if(!comp[i]) {
        for(int j=i; j<=MAX; j+=i) {
            if(!fac[j]) fac[j] = i;
            comp[j] = true;
        }
    }
}

void solve() {
    int n, m; cin >> n >> m;
    vector<int> a(m + 1);
    for(int i=0; i<m; i++) cin >> a[i];
    reverse(a.begin(), a.end());
    vector<int> ans;
    ans.reserve(n);
    for(int k=1; k<=n; k++) {
        int sum = 1;
        for(int tmp = k; tmp > 1;) {
            int p = fac[tmp];
            while(tmp % p == 0) {
                tmp /= p;
                sum++;
            }
        }
        if(sum > m) {
            cout << "-1\n";
            return;
        }
        ans.push_back(a[sum]);
    }
    for(int x : ans) cout << x << " ";
    cout << "\n";
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

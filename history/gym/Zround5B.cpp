#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void one(int q, vector<int>& ans) {
    ans.push_back(q);
}
void two(int q, vector<int>& ans) {
    int a = (q + 1) / 2;
    int b = (q * (q + 1)) / 2;
    ans.push_back(a);
    ans.push_back(b);
}
void three(int q, vector<int>& ans) {
    one(q, ans);
    two(q, ans);
}

void solve() {
    int p, q; cin >> p >> q;
    vector<int> ans;
    if(p == 1) {
        one(q, ans);
    } else if(p == 2) {
        two(q, ans);
    } else if(p == 3) {
        three(q, ans);
    }
    cout << ans.size() << '\n';
    for(int x : ans) cout << x << ' ';
    cout << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
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

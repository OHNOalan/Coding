#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
const int MAXN = 1e5;

#define DEBUG false

vector<int> f;

void init() {
    for(int x = 1; x <= MAXN; x = (x + 1) * 2) {
        f.push_back(x);
    }
}

void solve() {
    int n; cin >> n;
    auto it = lower_bound(f.begin(), f.end(), n);
    cout << it - f.begin() + 1 << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
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

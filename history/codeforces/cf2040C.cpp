#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false
void solve() {
    ll n, k;
    cin >> n >> k;

    if (n <= 63 && k > (1LL << (n - 1))) {
        cout << -1 << '\n';
        return;
    }

    vector<int> ans(n);
    int l = 0, r = n - 1;
    k--;

    for (int i = 0; i < n - 1; i++) {
        if ((n - i - 2) <= 61 && k >= (1LL << (n - i - 2))) {
            ans[r] = i + 1;
            r--;
            k -= (1LL << (n - i - 2));
        } else {
            ans[l] = i + 1;
            l++;
        }
    }
    ans[l] = n;

    for (int i = 0; i < n; i++) {
        cout << ans[i] << (i == n-1 ? '\n' : ' ');
    }
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

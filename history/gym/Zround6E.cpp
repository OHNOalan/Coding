#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int bit_length(unsigned x)
{
    return (8*sizeof x) - std::countl_zero(x);
}

void solve() {
    int n; cin >> n;
    while(n & 1) n >>= 1;
    cout << max(0, bit_length(n) - 1) << '\n';
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

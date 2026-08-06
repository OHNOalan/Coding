#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void init() {}

void solve() {
    int n, k;
    std::cin >> n >> k;

    if (n == 1) {
        std::cout << 1 << "\n";
        std::cout << 1 << "\n";
        return;
    }

    if (k % 2 == 0) {
        std::cout << 3 << "\n";
        std::cout << 1 << " " << k << " " << k + 1 << "\n";
        return;
    }

    if (k == 1 || k == n) {
        std::cout << -1 << "\n";
        return;
    }

    std::cout << 5 << "\n";
    std::cout << 1 << " " << 2 << " " << k << " " << k + 1 << " " << k + 2 << "\n";
}

#define MULTICASE true
int main() {
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

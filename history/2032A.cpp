#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void init() {}

void solve() {
    int n;
    std::cin >> n;

    std::vector<int> a(2 * n);
    for (int i = 0; i < 2 * n; i++) {
        std::cin >> a[i];
    }

    int cnt = std::accumulate(a.begin(), a.end(), 0);

    int min = cnt % 2;
    int max = std::min(cnt, 2 * n - cnt);

    std::cout << min << " " << max << "\n";
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

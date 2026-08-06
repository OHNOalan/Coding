#include <bits/stdc++.h>

void solve() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &x: a) std::cin >> x;
    std::ranges::sort(a, std::greater<>());
    for (int i = 2; i < n; i++) {
        if (a[i] != a[i - 2] % a[i - 1]) {
            return void(std::cout << "-1\n");
        }
    }
    std::cout << a[0] << " " << a[1] << "\n";
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    int t = 1;
    std::cin >> t;
    while (t--) solve();
}

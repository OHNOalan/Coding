#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    // 如果你本地代码开启了 cin >> t，这里输出 1
    cout << 1 << "\n";

    int n = 100000;
    cout << n << "\n";

    mt19937 rng(42);
    for (int i = 0; i < n; i++) {
        // 生成偏大的 ai 值 (n/2 到 n 之间)，这能让 split/merge 深度最大化
        // 且打破历史版本的结构复用，强制 clone 大量节点
        int ai = n / 2 + rng() % (n / 2);
        cout << ai << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}

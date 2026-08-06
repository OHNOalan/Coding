#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> g(n + 1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> parity(n + 1);
    vector<int> remove, path;
    auto dfs = [&](auto &&dfs, int x, int fa, int d) -> bool {
        parity[x] = d;
        bool onPath = x == n;
        for (int y: g[x])
            if (y != fa) {
                onPath |= dfs(dfs, y, x, 1 - d);
            }
        if (!onPath)
            remove.push_back(x);
        else
            path.push_back(x);
        return onPath;
    };

    dfs(dfs, 1, 0, 0);
    reverse(path.begin(), path.end());
    for (int x: path) remove.push_back(x);
    remove.pop_back();

    assert(remove.size() == n - 1);

    // cout << "remove: ";
    // for (int x: remove) cout << x + 1 << " ";
    // cout << endl;

    vector<vector<int>> move;

    auto one = [&]() -> void { move.push_back({1}); };
    auto two = [&](int u) -> void { move.push_back({2, u}); };

    int par = 0, lst = false;
    for (int x: remove) {
        if (parity[x] != par) {
            if (lst) {
                one();
                one();
            }
            two(x);
        } else {
            one();
            two(x);
            par ^= 1;
        }
        lst = true;
    }

    assert(move.size() <= 3 * n);

    cout << move.size() << '\n';
    for (auto &m: move) {
        for (int x: m) cout << x << " ";
        cout << '\n';
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

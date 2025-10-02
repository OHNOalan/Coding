#include <bits/stdc++.h>

using namespace std;

const int N = int(2e5) + 99;

int n, m, x;
char c;

struct Node {
    int val;
    Node *left, *right;
    Node(int _val, Node *_left, Node *_right) : val(val), left(_left), right(_right) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; i++) { cin >> a[i]; }
    sort(a.begin(), a.end());
    auto dfs = [&](auto &&dfs, int l, int r) -> Node * {
        if (l >= r)
            return nullptr;
        int m = (l + r) / 2;
        Node *root = new Node{a[m], dfs(dfs, l, m - 1), dfs(dfs, m + 1, r)};
    };
    Node *root = dfs(dfs, 0, n);
    return 0;
}

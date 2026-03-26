#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    map<int, vector<pair<int, int>>> mp;
    for(int i=0; i<m; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        assert(y1 == y2);
        mp[y1].emplace_back(x1, x2);
    }
    vector<int> a(n+1);
    iota(a.begin(), a.end(), 0);
    for(auto& [y, ps] : mp) {
        for(auto& [x1, x2] : ps) {
            swap(a[x1], a[x2]);
        }
    }
    vector<int> b(n+1);
    for(int i=1; i<=n; i++) {
        b[a[i]] = i;
    }
    for(int i=1; i<=n; i++) {
        cout << b[i] << '\n';
    }
    return 0;
} 

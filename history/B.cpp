#include <bits/stdc++.h>
using namespace std;

void solve() {
    string s1, s2; int m;
    cin >> s1 >> s2 >> m;
    unordered_map<string, int> nameToIdx;
    nameToIdx[s1] = 0, nameToIdx[s2] = 1;
    int start = 0, end = 1;
    vector<vector<pair<int,double>>> g(2);
    for(int i=0; i<m; i++) {
        int a1, a2;
        string name1, name2;
        cin >> a1 >> name1 >> a2 >> name2;
        if(nameToIdx.find(name1) == nameToIdx.end()) {
            nameToIdx[name1] = nameToIdx.size();
            g.push_back({});
        }
        if(nameToIdx.find(name2) == nameToIdx.end()) {
            nameToIdx[name2] = nameToIdx.size();
            g.push_back({});
        }
        int id1 = nameToIdx[name1], id2 = nameToIdx[name2];
        double ratio = (double) a2 / a1;
        g[id1].emplace_back(id2, ratio);
    }
    // for(auto &row : g) {
    //     for(auto [id, ratio] : row)
    //         cout << id << " " << ratio << " | ";
    //         cout << endl;
    // }

    int n = g.size();
    vector<float> dist(n, INT_MAX);
    vector<vector<pair<int,float>>> rg(n);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    dist[start] = 1;
    pq.emplace(1, start);
    while(!pq.empty()) {
        auto [ratio, x] = pq.top();
        pq.pop();
        for(auto [y, w] : g[x]) {
            double new_ratio = ratio * w;
            if(abs(new_ratio - dist[y]) <= 1e-6) {
                rg[y].emplace_back(x, w);
            } else if(new_ratio + 1e-5 < dist[y]) {
                rg[y].clear();
                dist[y] = new_ratio;
                rg[y].emplace_back(x, w);
                pq.emplace(new_ratio, y);
            }
        }
    }

    // int id = 0;
    // for(auto &row : rg) {
    //     cout << id++ << ": ";
    //     for(auto [id, ratio] : row)
    //         cout << id << " " << ratio << " | ";
    //         cout << endl;
    // }

    vector<unordered_map<float, int>> memo(n);

    auto dfs = [&](auto&& dfs, int x, double ratio) {
        // cout << "x: " << x << " ratio: " << ratio << endl;
        if(abs(ratio - 1.0) <= 1e-6) {
            // cout << "why not ? " << ratio << endl;
            return 1;
        }
        if(memo[x].count(ratio)) return memo[x][ratio];
        int res = 0;
        for(auto [y, w] : rg[x]) {
            double new_ratio = ratio / w;
            res += dfs(dfs, y, new_ratio);
        }
        // cout << "x: " << x << " res: " << res << endl;
        return memo[x][ratio] = res > 0 ? res : 0;
    };

    int ways = dfs(dfs, end, dist[end]);
    double ratio = dist[end];
    int digits = (ratio == 0.0) ? 1 : (int)std::log10(std::abs(ratio)) + 1;
    int precision = max(0, 5 - digits);
    printf("%.*f ", precision, ratio);
    // cout << setprecision(5) << ratio;
    cout << ways << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n; cin >> n;
  int id = 1;
  while(n--) {
    cout << "Case " << id++ << ": ";
    solve();
  }
  return 0;
}
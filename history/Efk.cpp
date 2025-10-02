#include <bits/stdc++.h>
using namespace std;

static constexpr int dirs[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void solve(int n, int m) {
    cout << "Solve " << n << " " << m << endl;
    vector<vector<int>> h(n, vector<int>(m));
    unordered_map<int, int> hCnt;
    int mx = 0;
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) {
            int height;
            cin >> height;
            h[i][j] = height;
            hCnt[height]++;
            mx = max(mx, height);
        }
    

    // for (int i = 0; i < n; i++){
    //   for (int j = 0; j < m; j++) 
    //     cout << h[i][j] << " ";
    //   cout << endl;
    // }
    // cout << "A" << endl;
    int ocean = 0;
    int cur_depth = 0;
    while(cur_depth <= mx) {
        // cout << "B" << endl;
        ocean += hCnt[cur_depth];
        int sx = -1, sy = -1;
        for(int i=0; i<n; i++) {
            for(int j=0; j<m; j++) {
                if(h[i][j] > cur_depth) {
                    sx = i, sy = j;
                    goto OUT;
                }
            }
        }
        OUT:
        if(sx == -1 && sy == -1) {
            cout << "Island never splits." << endl;
            return;
        }
        // cout << "C " << sx << sy << endl;
        queue<pair<int,int>> q;
        unordered_set<int> vis;
        vis.insert(sx * m + sy);
        q.emplace(sx, sy);
        int land = 0;
        while(!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            land++;
            for(int i=0; i<4; i++) {
                int nx = x + dirs[i][0];
                int ny = y + dirs[i][1];
                if(nx < 0 || nx >= n || ny < 0 || ny >= m || vis.count(nx * m + ny) || h[nx][ny] <= cur_depth) continue;
                vis.insert(nx * m + ny);
                q.emplace(nx, ny);
            }
        }
        if(n * m - land - ocean != 0) {
            cout << "Island splits when ocean rises " << cur_depth << " feet." << land << " " << ocean << endl;
            return;
        }
        cur_depth++;
    }
    cout << "Island never splits." << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  string s;
  int n, m;
  while (true) {
    cin >> n >> m;
    if (n == 0 && m == 0) break;
    solve(n, m);
  }
  return 0;
}
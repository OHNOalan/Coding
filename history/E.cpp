#include <bits/stdc++.h>
using namespace std;

static constexpr int dirs[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void solve(int n, int m) {
    auto getIdx = [&](int i, int j) -> int { return i * m + j; };
    // cout << "Solve " << n << " " << m << endl;
    vector<vector<int>> h(n, vector<int>(m));
    // unordered_map<int, int> hCnt;
    int mx = 0;
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) {
            int height;
            cin >> height;
            h[i][j] = height;
            // hCnt[height]++;
            mx = max(mx, height);
        }
    

    // for (int i = 0; i < n; i++){
    //   for (int j = 0; j < m; j++) 
    //     cout << h[i][j] << " ";
    //   cout << endl;
    // }
    // cout << "A" << endl;
    int cur_depth = 0;
    while(cur_depth <= mx) {
        // cout << "B" << endl;
        // cout << "C " << sx << sy << endl;
        unordered_set<int> vis;
        queue<pair<int,int>> q2;
        for(int j=0; j<m; j++) {
            if(h[0][j] <= cur_depth) {
                q2.emplace(0, j);
                vis.insert(getIdx(0, j));
            }
            if(h[n-1][j] <= cur_depth) {
                q2.emplace(n-1, j);
                vis.insert(getIdx(n-1, j));
            }
        }
        for(int i=1; i<n-1; i++) {
            if(h[i][0] <= cur_depth) {
                q2.emplace(i, 0);
                vis.insert(getIdx(i, 0));
            }
            if(h[i][m-1] <= cur_depth) {
                q2.emplace(i, m-1);
                vis.insert(getIdx(i, m-1));
            }
        }
        
        int ocean = 0;
        while(!q2.empty()) {
            auto [x, y] = q2.front();
            q2.pop();
            ocean++;
            for(int i=0; i<4; i++) {
                int nx = x + dirs[i][0];
                int ny = y + dirs[i][1];
                if(nx < 0 || nx >= n || ny < 0 || ny >= m || vis.count(getIdx(nx, ny)) || h[nx][ny] > cur_depth) continue;
                vis.insert(getIdx(nx, ny));
                q2.emplace(nx, ny);
            }
        }

        queue<pair<int,int>> q;
        int sx = -1, sy = -1;
        for(int i=0; i<n; i++) {
            for(int j=0; j<m; j++) {
                if(!vis.count(getIdx(i, j))) {
                    sx = i, sy = j;
                    goto OUT;
                }
            }
        }
        OUT:
        if(sx == -1 && sy == -1) break;

        vis.insert(getIdx(sx, sy));
        q.emplace(sx, sy);
        int land = 0;
        while(!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            land++;
            for(int i=0; i<4; i++) {
                int nx = x + dirs[i][0];
                int ny = y + dirs[i][1];
                if(nx < 0 || nx >= n || ny < 0 || ny >= m || vis.count(getIdx(nx, ny))) continue;
                vis.insert(getIdx(nx, ny));
                q.emplace(nx, ny);
            }
        }
        if(land + ocean < n * m) {
            cout << "Island splits when ocean rises " << cur_depth << " feet."<< endl;
            // cout << land << " " << ocean << endl;
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
  int id = 1;
  while (true) {
    cin >> n >> m;
    if (n == 0 && m == 0) break;
    cout << "Case " << id++ << ": ";
    solve(n, m);
  }
  return 0;
}
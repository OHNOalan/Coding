#include <bits/stdc++.h>
using namespace std;

const int N = 101;
void solve(int v, int o, int m) {
    vector<vector<int>> grid(N, vector<int>(N));
    for(int i=0; i<v; i++) {
        int x, y; cin >> x >> y;
        grid[x][y] = 1;
    }
    for(int i=0; i<o; i++) {
        int x, y; cin >> x >> y;
        grid[x][y] = 2;
    }
    for(int i=0; i<m; i++) {
        int x, y; cin >> x >> y;
        grid[x][y] = 3;
    }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  string s;
  int v, o, m;
  int id = 1;
  while (true) {
    cin >> v >> o >> m;
    if (v == 0 && o == 0 && m == 0) break;
    cout << "Case " << id++ << ":" << endl;
    solve(v, o, m);
  }
  return 0;
}
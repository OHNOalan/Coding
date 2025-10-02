#include <bits/stdc++.h>
using namespace std;

constexpr int N = 1e6;

int h[N * 2], e[N * 6], ne[N * 6], f[N * 6], idx, S, T, cur[N * 2], d[N * 2];
bool vis[N * 2];
void add(int a, int b, int c) {
 e[idx] = b, ne[idx] = h[a], f[idx] = c, h[a] = idx++;
 e[idx] = a, ne[idx] = h[b], f[idx] = 0, h[b] = idx++;
}
bool bfs() {
 memset(d, -1, sizeof(d));
 memset(vis, 0, sizeof(vis));
 queue<int> q;
 q.push(S);
 vis[S] = 1, d[S] = 0, cur[S] = h[S];
 while (q.size()) {
  int t = q.front();
  q.pop();
  for (int i = h[t]; ~i; i = ne[i]) {
   int j = e[i];
   if (vis[j] || !f[i]) continue;
   d[j] = d[t] + 1, vis[j] = 1, cur[j] = h[j];
   if (j == T) return 1;
   q.push(j);
  }
 }
 return 0;
}
int dfs(int x, int limit) {
 if (x == T) return limit;
 int flow = 0;
 for (int i = cur[x]; (~i) && flow < limit; i = ne[i]) {
  int j = e[i];
  cur[x] = i;
  if (d[j] != d[x] + 1 || !f[i]) continue;
  int k = dfs(j, min(f[i], limit - flow));
  if (!k) d[j] = -1;
  f[i] -= k;
  f[i ^ 1] += k;
  flow += k;
 }
 return flow;
}
int dinic() {
 int ans = 0, flow;
 while (bfs()) while (flow = dfs(S, 1e9)) ans += flow;
 return ans;
}
memset(h,-1,sizeof h);
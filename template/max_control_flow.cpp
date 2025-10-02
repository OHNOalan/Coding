#include <bits/stdc++.h>

// #define DEBUG

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;
using u128 = unsigned __int128;
constexpr int inf32 = 1E9 + 7;
constexpr i64 inf64 = 1E18 + 7;

struct MCFGraph{
    struct Edge{
        int v, cap, w;
        Edge(int _v, int _cap, int _w) : v(_v), cap(_cap), w(_w) {}
    };

    const int n;
    std::vector<Edge> e;
    std::vector<std::vector<int>> g;
    std::vector<i64> h, dis;    // h 为 势 数组
    std::vector<int> pre;
    std::vector<bool> vis;


    void spfa(const int s, const int t) {
        static std::queue<int> q;
        h.assign(n, std::numeric_limits<i64>::max());
        q.push(s);
        h[s] = 0;
        while (q.size()) {
            int u = q.front();
            q.pop();
            vis[u] = false;
            for (int i : g[u]) {
                const auto& [v, cap, w] = e[i];
                if (cap > 0 && h[v] > h[u] + w) {
                    h[v] = h[u] + w;
                    if (!vis[v])
                        q.push(v), vis[v] = true;
                }
            }
        }
    }

    bool dijkstra(const int s, const int t){
        static std::priority_queue<std::pair<i64, int>, std::vector<std::pair<i64, int>>, std::greater<std::pair<i64, int>>> pq;

        dis.assign(n, std::numeric_limits<i64>::max());
        pre.assign(n, -1);

        dis[s] = 0;
        pq.emplace(0, s);

        while (pq.size()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (dis[u] < d) continue;

            for (int i : g[u]) {
                const auto& [v, cap, w] = e[i];
                if (cap > 0 && dis[v] > w + d + h[u] - h[v]) {
                    dis[v] = w + d + h[u] - h[v];
                    pre[v] = i;
                    pq.emplace(dis[v], v);
                }
            }
        }

        return dis[t] < std::numeric_limits<i64>::max();
    }

    MCFGraph(int _n) : n(_n), g(n), vis(n)
    {}

    void addEdge(int u, int v, int c, int f) { // 最大流
        g[u].push_back(e.size());
        e.emplace_back(v, c, f);
        g[v].push_back(e.size());
        e.emplace_back(u, 0, -f);
    }

    std::pair<int, i64> flow(const int s, const int t) {
        int flow = 0;
        i64 cost = 0;
        h.assign(n, 0);
        spfa(s, t);

        while (dijkstra(s, t)) {
            // 更新h 为实际dis
            for (int i = 0; i < n; ++ i)    h[i] += dis[i];
            int aug = std::numeric_limits<int>::max();
            for (int i = t; i != s; i = e[pre[i] ^ 1].v)
                aug = std::min(aug, e[pre[i]].cap);
            for (int i = t; i != s; i = e[pre[i] ^ 1].v)
                e[pre[i]].cap -= aug, e[pre[i] ^ 1].cap += aug;
            flow += aug;
            cost += (i64)aug * h[t];
        }
        return std::make_pair(flow, cost);
    }
};

void solve() {







}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

#ifdef DEBUG
    int START = clock();
    frewen("in.txt", "r", stdin);
    frewen("out.txt", "w", stdout);
#endif
    int t = 1;
    // std::cin >> t;

    while (t --) {
        solve();
    }
#ifdef DEBUG
    std::cerr << "run-time: " << clock() - START << '\n';
#endif
    return 0;
}
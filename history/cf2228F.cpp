#include <algorithm>
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

const int MAXN = 2e5 + 5;
int dist[MAXN];
int a[MAXN];
int dp[MAXN];
ll v_s[MAXN];
int v_d[MAXN];
int pre[MAXN];
int order[MAXN];
int fa[MAXN];
bool vis[MAXN];

int head[MAXN], next_edge[MAXN * 2], to_node[MAXN * 2], edge_cnt;
int child_start[MAXN], child_count[MAXN], sorted_children[MAXN * 2];
int q_buf[MAXN];

#define BUF_SIZE (1 << 18)
char buf[BUF_SIZE];
int buf_pos = 0, buf_len = 0;

inline char next_char() {
    if (buf_pos >= buf_len) [[unlikely]] {
        buf_len = fread(buf, 1, BUF_SIZE, stdin);
        buf_pos = 0;
        if (buf_len == 0) return EOF;
    }
    return buf[buf_pos++];
}

inline int read_int() {
    int x = 0, c;
    while ((c = next_char()) < '0' || c > '9') {
        if (c == EOF) return 0;
    }
    x = c - '0';
    while ((c = next_char()) >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
    }
    return x;
}

inline void add_edge(int u, int v) {
    to_node[edge_cnt] = v;
    next_edge[edge_cnt] = head[u];
    head[u] = edge_cnt++;
}

void solve() {
    int n, k;
    n = read_int();
    k = read_int();
    for (int i = 0; i < n; i++) a[i] = read_int();
    fill(head, head + n, -1);
    edge_cnt = 0;
    for (int i = 1; i < n; i++) {
        int u, v;
        u = read_int();
        v = read_int();
        --u, --v;
        add_edge(u, v);
        add_edge(v, u);
    }

    auto get_furthest = [&](int start, int &max_dist) -> int {
        fill(dist, dist + n, -1);
        int head_q = 0, tail_q = 0;
        q_buf[tail_q++] = start;
        dist[start] = 0;
        int furthest = start;
        while (head_q < tail_q) {
            int u = q_buf[head_q++];
            if (dist[u] > dist[furthest]) furthest = u;
            for (int e = head[u]; e != -1; e = next_edge[e]) {
                int v = to_node[e];
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q_buf[tail_q++] = v;
                }
            }
        }
        max_dist = dist[furthest];
        return furthest;
    };

    int d;
    get_furthest(get_furthest(0, d), d);
    if (d < k) {
        return void(cout << "-1\n"); // 直径小于 k，直接剪枝
    }

    int order_cnt = 0;
    fill(fa, fa + n, -1);
    fill(vis, vis + n, false);

    int head_q = 0, tail_q = 0;
    q_buf[tail_q++] = 0;
    vis[0] = true;
    while (head_q < tail_q) {
        int u = q_buf[head_q++];
        order[order_cnt++] = u;
        for (int e = head[u]; e != -1; e = next_edge[e]) {
            int v = to_node[e];
            if (!vis[v]) {
                vis[v] = true;
                fa[v] = u;
                q_buf[tail_q++] = v;
            }
        }
    }

    int total_children = 0;
    for (int i = 0; i < n; i++) {
        child_start[i] = total_children;
        child_count[i] = 0;
        for (int e = head[i]; e != -1; e = next_edge[e]) {
            int v = to_node[e];
            if (v != fa[i]) {
                sorted_children[total_children++] = v;
                child_count[i]++;
            }
        }
    }

    vector<ll> s(n);
    for (int i = n - 1; i >= 0; --i) {
        int x = order[i];
        s[x] = a[x];
        int start = child_start[x];
        int count = child_count[x];
        for (int j = 0; j < count; j++) {
            s[x] += s[sorted_children[start + j]];
        }
        sort(sorted_children + start, sorted_children + start + count,
             [&](int u, int v) -> bool { return s[u] < s[v]; });
    }

    auto check = [&](ll bound) -> bool {
        ll limit = s[0] - bound;
        for (int idx = n - 1; idx >= 0; --idx) {
            int x = order[idx];
            dp[x] = (s[x] >= bound) ? 0 : -1;

            int m = 0;
            int start = child_start[x];
            int count = child_count[x];
            for (int j = 0; j < count; j++) {
                int y = sorted_children[start + j];
                int dy = dp[y];
                if (dy != -1) {
                    if (s[0] - s[y] >= bound && dy + 1 >= k) {
                        return true;
                    }
                    if (s[x] - s[y] >= bound) {
                        dp[x] = max(dp[x], dy + 1);
                    }
                    v_s[m] = s[y];
                    v_d[m] = dy;
                    m++;
                }
            }

            if (m >= 2) {
                pre[0] = 0;
                for (int i = 0; i < m; i++) pre[i + 1] = max(pre[i], v_d[i]);
                int pos = -1;
                for (int i = m - 1; i > 0; --i) {
                    ll target = limit - v_s[i];
                    while (pos + 1 < m && v_s[pos + 1] <= target) ++pos;

                    int valid_idx = pos + 1 < i ? pos + 1 : i;
                    if (pos >= 0 && v_d[i] + pre[valid_idx] + 2 >= k) {
                        return true;
                    }
                }
            }
        }
        return false;
    };

    if (!check(0)) {
        return void(cout << "-1\n");
    }

    ll l = 0, r = s[0] / (k + 1) + 1;
    while (l + 1 < r) {
        ll m = (l + r) >> 1;
        ((check(m)) ? l : r) = m;
    }
    cout << l << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = read_int();
    while (t--) solve();
}

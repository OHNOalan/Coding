#include <bits/stdc++.h>

using namespace std;

const int INF = INT_MAX / 2;

void solve() {
    int n;
    cin >> n;

    int N = n + 1;
    vector<int> parent(N, -1);
    for (int v = 1; v <= n; ++v) {
        cin >> parent[v];
        --parent[v];
    }

    vector<int> tin(N, 0);
    for (int v = 1; v <= n; ++v) tin[v] = v;

    vector<vector<int>> out(N);
    vector<int> from, to;
    from.reserve(2 * n);
    to.reserve(2 * n);

    auto add_edge = [&](int u, int v) -> void {
        int id = (int) from.size();
        from.push_back(u);
        to.push_back(v);
        out[u].push_back(id);
        from.push_back(v);
        to.push_back(u);
        out[v].push_back(id ^ 1);
    };

    for (int v = 1; v <= n; ++v) add_edge(parent[v], v);

    int M2 = 2 * n;
    vector<int> prev(M2), cur(M2, INF);
    for (int e = 0; e < M2; ++e) prev[e] = tin[to[e]];

    int maxMove = 0;
    while ((1 << maxMove) <= N) ++maxMove;
    ++maxMove;

    vector<int> earliest(maxMove + 2, INF);
    earliest[1] = 0;

    for (int step = 1; step <= maxMove; ++step) {
        vector<int> bestV1(N, INF), bestV2(N, INF), bestV3(N, INF);
        vector<int> bestE1(N, -1), bestE2(N, -1), bestE3(N, -1);

        auto push_best = [&](int x, int val, int eid) -> void {
            if (val < bestV1[x]) {
                bestV3[x] = bestV2[x], bestE3[x] = bestE2[x];
                bestV2[x] = bestV1[x], bestE2[x] = bestE1[x];
                bestV1[x] = val, bestE1[x] = eid;
            } else if (val < bestV2[x]) {
                bestV3[x] = bestV2[x], bestE3[x] = bestE2[x];
                bestV2[x] = val, bestE2[x] = eid;
            } else if (val < bestV3[x]) {
                bestV3[x] = val, bestE3[x] = eid;
            }
        };

        for (int x = 0; x < N; ++x) {
            for (int eid: out[x]) push_best(x, prev[eid], eid);
        }

        int rootEarliest = INF;
        for (int x = 0; x < N; ++x) {
            if (bestV2[x] >= INF) continue;
            rootEarliest = min(rootEarliest, max(tin[x], bestV2[x]));
        }
        earliest[step + 1] = rootEarliest;

        bool anyFinite = false;
        for (int e = 0; e < M2; ++e) {
            int u = to[e];
            int forbidden = e ^ 1;
            int need;
            if (bestE1[u] == forbidden || bestE2[u] == forbidden) {
                need = bestV3[u];
            } else {
                need = bestV2[u];
            }
            if (need >= INF) {
                cur[e] = INF;
            } else {
                cur[e] = max(tin[u], need);
                anyFinite = true;
            }
        }

        if (!anyFinite && rootEarliest >= INF) break;
        swap(prev, cur);
    }

    vector<int> ans(n, 1);
    int best = 1;
    for (int q = 1; q <= n; ++q) {
        while (best + 1 < (int) earliest.size() && earliest[best + 1] <= q) ++best;
        ans[q - 1] = best;
    }

    for (int x: ans) cout << x << ' ';
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}

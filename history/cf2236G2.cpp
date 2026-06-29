#include <bits/stdc++.h>

using namespace std;

const int LG = 21;

struct JumpData {
    int par;
    int cnt;
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> par(n + 1), depth(n + 1);
    [&](this auto self, int x, int fa) -> void {
        depth[x] = depth[fa] + 1;
        par[x] = fa;
        for (int y: g[x]) {
            if (y != fa) {
                self(y, x);
            }
        }
    }(1, 0);
    int M = bit_width(unsigned(n + 1));
    vector<int> upPar(M * (n + 1));
    vector<int> upNonZeroCnt(M * (n + 1));
    vector<JumpData> up((n + 1) * M);
    auto get_up = [&](int d, int i) -> JumpData & { return up[i * M + d]; };
    for (int i = 0; i <= n; i++) {
        auto &jump = get_up(0, i);
        jump.par = par[i];
        jump.cnt = a[i] != 0;
    }
    for (int d = 1; d < M; d++) {
        for (int i = 1; i <= n; i++) {
            auto &jump = get_up(d, i);
            auto &p_jump = get_up(d - 1, i);
            auto &p_p_jump = get_up(d - 1, p_jump.par);
            jump.par = p_p_jump.par;
            jump.cnt = p_p_jump.cnt + p_jump.cnt;
        }
    }
    vector<pair<int, int>> upFirstNonZero(n + 1);
    for (int i = 1; i <= n; i++) {
        int x = par[i], cnt = 0;
        for (int d = M - 1; d >= 0 && x != 0; d--) {
            if (get_up(d, x).cnt == 0) {
                x = get_up(d, x).par;
                cnt |= (1 << d);
            }
        }
        upFirstNonZero[i] = make_pair(x, cnt);
    }
    vector<int> bad(n + 1);
    for (int i = 1; i <= n; i++) {
        int x = i;
        int Xor = 0, Sum = 0;
        while (x != 0) {
            if ((Xor ^ a[x]) != (Sum + a[x])) {
                bad[i] = x;
                break;
            }
            Xor ^= a[x];
            Sum += a[x];
            x = upFirstNonZero[x].first;
        }
    }
    vector<long long> sum_vertical(n + 1, 0);
    [&](this auto self, int x, int fa) -> void {
        long long current_contribution = depth[x] - depth[bad[x]];
        sum_vertical[x] = sum_vertical[fa] + current_contribution;
        for (int y: g[x]) {
            if (y != fa) {
                self(y, x);
            }
        }
    }(1, 0);

    auto get_vertical_hospitable = [&](int bottom, int top) -> long long {
        if (depth[bottom] <= depth[top]) return 0;
        return sum_vertical[bottom] - sum_vertical[top];
    };

    vector<pair<int, int>> pathX, pathY;
    pathX.reserve(LG), pathY.reserve(LG);
    while (q--) {
        int x, y;
        cin >> x >> y;
        if (depth[x] < depth[y]) swap(x, y);
        int z = x;
        int cntX = 0, cntY = 0;
        {
            int xx = x, yy = y;
            for (int d = M - 1; d >= 0; d--) {
                if (depth[xx] - (1 << d) >= depth[yy]) {
                    auto &jump = get_up(d, xx);
                    cntX += jump.cnt;
                    xx = jump.par;
                }
            }
            if (xx == yy) {
                z = xx;
            } else {
                for (int d = M - 1; d >= 0; d--) {
                    auto &p_xx = get_up(d, xx);
                    auto &p_yy = get_up(d, yy);
                    if (p_xx.par != p_yy.par) {
                        cntX += p_xx.cnt;
                        cntY += p_yy.cnt;
                        xx = p_xx.par;
                        yy = p_yy.par;
                    }
                }
                cntX += get_up(0, xx).cnt;
                cntY += get_up(0, yy).cnt;
                z = get_up(0, xx).par;
            }
        }

        cntX = max(0, cntX - LG);
        cntY = max(0, cntY - LG);

        int xx = x, yy = y;
        for (int d = M - 1; d >= 0 && cntX; d--) {
            auto &jump = get_up(d, xx);
            if (jump.cnt <= cntX) {
                cntX -= jump.cnt;
                xx = jump.par;
            }
        }
        for (int d = M - 1; d >= 0 && cntY; d--) {
            auto &jump = get_up(d, yy);
            if (jump.cnt <= cntY) {
                cntY -= jump.cnt;
                yy = jump.par;
            }
        }
        pathX.clear(), pathY.clear();
        auto getPath = [&](int x, int z, vector<pair<int, int>> &path) -> void {
            while (x != z) {
                path.push_back({a[x], 1});
                auto [nx, cnt] = upFirstNonZero[x];
                if (depth[nx] >= depth[z]) {
                    if (cnt != 0) {
                        path.push_back({0, cnt});
                    }
                    x = nx;
                } else {
                    if (depth[x] - depth[z] - 1 != 0) {
                        path.push_back({0, depth[x] - depth[z] - 1});
                    }
                    x = z;
                }
            }
        };
        getPath(xx, z, pathX);
        getPath(yy, z, pathY);
        pathX.push_back({a[z], 1});
        auto &path = pathX;
        path.insert(path.end(), pathY.rbegin(), pathY.rend());
        int m = path.size();
        long long ans = get_vertical_hospitable(x, xx) + get_vertical_hospitable(y, yy);
        for (int i = 0, j = 0, Xor = 0, Sum = 0, tc = 0; j < m; j++) {
            auto [v, c] = path[j];
            Xor ^= v;
            Sum += v;
            while (Xor != Sum) {
                Xor ^= path[i].first;
                Sum -= path[i].first;
                tc -= path[i].second;
                i++;
            }
            ans += 1ll * c * (2 * tc + 1 + c) / 2;
            tc += c;
        }
        cout << ans << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

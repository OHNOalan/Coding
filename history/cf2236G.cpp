#include <bit>
#include <bits/stdc++.h>

using namespace std;

const int LG = 24;

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
    vector<vector<int>> upPar(M, vector<int>(n + 1));
    vector<vector<int>> upNonZeroCnt(M, vector<int>(n + 1));
    vector<vector<int>> upXor(M, vector<int>(n + 1));
    vector<vector<int>> upSum(M, vector<int>(n + 1));
    upPar[0] = par;
    transform(a.begin(), a.end(), upNonZeroCnt[0].begin(), [](int x) -> int { return x != 0; });
    upXor[0] = a;
    upSum[0] = a;
    for (int d = 1; d < M; d++) {
        for (int i = 1; i <= n; i++) {
            upPar[d][i] = upPar[d - 1][upPar[d - 1][i]];
            upNonZeroCnt[d][i] = upNonZeroCnt[d - 1][upPar[d - 1][i]] + upNonZeroCnt[d - 1][i];
            upXor[d][i] = upXor[d - 1][upPar[d - 1][i]] ^ upXor[d - 1][i];
            upSum[d][i] = upSum[d - 1][upPar[d - 1][i]] + upSum[d - 1][i];
        }
    }
    vector<pair<int, int>> upFirstNonZero(n + 1);
    for (int i = 1; i <= n; i++) {
        int x = par[i], cnt = 0;
        assert(depth[x] < depth[i]);
        for (int d = M - 1; d >= 0 && x != 0; d--) {
            if (upNonZeroCnt[d][x] == 0) {
                x = upPar[d][x];
                cnt |= (1 << d);
            }
        }
        assert(depth[x] < depth[i]);
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
                    cntX += upNonZeroCnt[d][xx];
                    xx = upPar[d][xx];
                }
            }
            if (xx == yy) {
                z = xx;
            } else {
                for (int d = M - 1; d >= 0; d--) {
                    if (upPar[d][xx] != upPar[d][yy]) {
                        cntX += upNonZeroCnt[d][xx];
                        cntY += upNonZeroCnt[d][yy];
                        xx = upPar[d][xx];
                        yy = upPar[d][yy];
                    }
                }
                assert(par[xx] == par[yy]);
                cntX += upNonZeroCnt[0][xx];
                cntY += upNonZeroCnt[0][yy];
                z = par[xx];
            }
        }

        cntX = max(0, cntX - LG);
        cntY = max(0, cntY - LG);

        int xx = x, yy = y;
        for (int d = M - 1; d >= 0 && cntX; d--) {
            if (upNonZeroCnt[d][xx] <= cntX) {
                cntX -= upNonZeroCnt[d][xx];
                xx = upPar[d][xx];
            }
        }
        for (int d = M - 1; d >= 0 && cntY; d--) {
            if (upNonZeroCnt[d][yy] <= cntY) {
                cntY -= upNonZeroCnt[d][yy];
                yy = upPar[d][yy];
            }
        }

        vector<pair<int, int>> pathX, pathY;
        auto getPath = [&](int x, int z, vector<pair<int, int>> &path) -> void {
            assert(depth[x] >= depth[z]);
            while (x != z) {
                path.push_back({a[x], 1});
                auto [nx, cnt] = upFirstNonZero[x];
                assert(nx != x && depth[x] > depth[nx]);
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
                assert(i < j);
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

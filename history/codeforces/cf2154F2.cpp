#include <bits/stdc++.h>

using namespace std;

using i64 = long long;
const int INF = 1e9;
const int N = 1e6 + 1;
const int MOD = 998244353;
i64 F[N], INVF[N];

i64 qmul(i64 a, i64 b) {
    i64 res = 1;
    while (b) {
        if (b & 1)
            res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

void init() {
    F[0] = 1;
    for (int i = 1; i < N; i++) { F[i] = F[i - 1] * i % MOD; }
    INVF[N - 1] = qmul(F[N - 1], MOD - 2);
    for (int i = N - 1; i > 0; i--) { INVF[i - 1] = INVF[i] * i % MOD; }
    assert(INVF[0] == 1);
    assert(INVF[1] == 1);
}

i64 C(int n, int r) {
    if (r < 0 || r > n)
        return 0;
    return F[n] * INVF[r] % MOD * INVF[n - r] % MOD;
}

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) {
        cin >> x;
        if (x != -1)
            x--;
    }

    bool fixed = true;
    for (int i = 0; i < n; i++)
        if (a[i] != -1 && a[i] != i)
            fixed = false;

    if (fixed) {
        vector<i64> prod(n + 1);
        prod[0] = 1;
        for (int i = 1; i <= n; i++) prod[i] = prod[i - 1] * 2 % MOD;

        int last = -1;
        i64 ans = 0;
        for (int i = 0; i <= n; i++) {
            if (i == n || a[i] != -1) {
                int len = i - last - 1;
                ans += prod[len] - len - 1;
                last = i;
            }
        }

        ans += 1;
        ans %= MOD;
        cout << ans << "\n";
        return;
    }

    vector<bool> side(n);
    bool found = false;
    for (int i = 0; i < n; i++) {
        if (a[i] == -1)
            continue;

        if (a[i] < i)
            side[i] = false; // left side
        if (a[i] > i)
            side[i] = true; // right side
        if (a[i] == i)
            side[i] = found;
        if (a[i] != i)
            found = true;
    }

    int last = -1;
    vector<array<int, 2>> ranges;
    for (int i = 0; i < n; i++) {
        if (a[i] == -1)
            continue;

        if (last == -1) {
            last = i;
            continue;
        }

        if (side[i] && !side[last]) { // left -> right gap
            int mn = last - a[last]; // min right already placed
            int mx = mn + i - last; // max right already placed
            ranges.push_back({a[i] - mx, a[i] - mn + 1});
        }

        if (!side[i] && side[last]) { // right -> left gap
            int mx = i - a[i]; // max right placed by last
            int mn = mx - (i - last); // min right placed by last
            ranges.push_back({a[last] - mx, a[last] - mn + 1});
        }

        last = i;
    }

    int l = 0, r = n - 1;
    for (auto [u, v]: ranges) {
        l = max(l, u);
        r = min(r, v);
    }


    i64 coef = 1;
    vector<i64> score(n); // score[i] = contribution when splitting at i
    for (int i = l; i <= r; i++) score[i] = 1;


    last = -1;
    for (int i = 0; i < n; i++) {
        if (a[i] == -1)
            continue;

        bool prev = false;
        if (last != -1)
            prev = side[last];
        int spaces = i - last - 1;

        if (side[i] && prev || !side[i] && !prev) {
            int cnt = 0;
            if (last == -1)
                cnt = a[i];
            else
                cnt = a[i] - a[last] - 1;

            coef = coef * C(spaces, cnt) % MOD;
        }

        if (side[i] && !prev) { // left -> right
            for (int j = l; j <= r; j++) {
                int placed = 0;
                if (last != -1)
                    placed = last - a[last]; // how many right i have placed by last

                int need = a[i] - (j + placed); // j + placed is the next one i need to place after last
                score[j] = score[j] * C(spaces, need) % MOD;
            }
        }

        if (!side[i] && prev) { // right -> left
            for (int j = l; j <= r; j++) {
                int big = i - a[i] + j - 1; // last right to be placed by i

                int need = big - a[last];
                score[j] = score[j] * C(spaces, need) % MOD;
            }
        }

        last = i;
    }

    for (int j = l; j <= r; j++) {
        if (!side[last]) {
            score[j] = score[j] * C(n - last - 1, j - a[last] - 1) % MOD;
        } else {
            score[j] = score[j] * C(n - last - 1, n - 1 - a[last]) % MOD;
        }
    }

    i64 ans = 0;
    for (int i = 0; i < n; i++) ans += score[i];
    ans %= MOD;
    ans = ans * coef % MOD;
    cout << ans << "\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();

    int t;
    cin >> t;
    while (t--) solve();
}

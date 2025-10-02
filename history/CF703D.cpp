using i64 = long long;
using ll = long long;
using i128 = __int128;
using u128 = unsigned __int128;
using u32 = unsigned;
using u64 = unsigned long long;
using f32 = double;
using f64 = long double;

#define uset unordered_set
#define umap unordered_map
#define vs vector<string>
#define vi vector<int>
#define vvi vector<vi>
#define vvvi vector<vvi>
#define vll vector<i64>
#define vvll vector<vll>
#define vvvll vector<vvll>
#define pii pair<int, int>
#define pll pair<i64, i64>
#define vpii vector<pii>
#define vpll vector<pll>
#define vvpii vector<vpii>
#define vvpll vector<vpll>
#define vz vector<Z>
#define vvz vector<vz>
#define vvvz vector<vvz>
#define pb push_back
#define eb emplace_back

#define ALL(x) (x).begin(), (x).end()
#define DIS(x) (x).erase(unique((x).begin(), (x).end()), (x).end())
#define SRT(x) sort((x).begin(), (x).end())
#define REV(x) reverse((x).begin(), (x).end())
#define FND(x, y) find((x).begin(), (x).end(), (y))
#define CNT(x, y) count((x).begin(), (x).end(), (y))
#define SIZ(x) (static_cast<int>(x.size()))
#define MIN(x) *min_element((x).begin(), (x).end())
#define MAX(x) *max_element((x).begin(), (x).end())
#define LWB(x, y) distance((x).begin(), lower_bound((x).begin(), (x).end(), (y)))
#define YES(x) cout << ((x) ? "YES\n" : "NO\n")
#define yes(x) cout << ((x) ? "yes\n" : "no\n")
#define Yes(x) cout << ((x) ? "Yes\n" : "No\n")

#define rep(i, x, y) for (int(i) = (x); (i) < (y); (i)++)
#define reps(i, x, y, s) for (int(i) = (x); (i) < (y); (i) += (s))
#define repr(i, x, y) for (int(i) = (x); (i) > (y); (i)--)
#define reprs(i, x, y, s) for (int(i) = (x); (i) > (y); (i) -= (s))
#define fi first
#define se second

#include <bits/stdc++.h>

using namespace std;

const int MAXN = int(1e6) + 1;

int n, m, l, r;
int a[MAXN], ans[MAXN], s1[MAXN];
vector<pair<int, int>> q[MAXN];
map<int, int> lst;

int bit[MAXN];
int sum(int x) {
    int sum = 0;
    while (x > 0) {
        sum ^= bit[x];
        x -= (x & -x);
    }
    return sum;
}
void upd(int x, int v) {
    if (x == 0)
        return;
    while (x <= n) {
        bit[x] ^= v;
        x += (x & -x);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;
    rep(i, 1, n + 1) {
        cin >> a[i];
        s1[i] = s1[i - 1] ^ a[i];
    }
    cin >> m;
    rep(i, 0, m) {
        cin >> l >> r;
        q[r].emplace_back(l, i);
    }

    for (int r = 1; r <= n; r++) {
        int x = a[r];
        if (lst.count(x))
            upd(lst[x], x);
        upd(r, x);
        lst[x] = r;
        for (int j = 0; j < q[r].size(); j++) {
            auto &[l, i] = q[r][j];
            ans[i] = sum(r) ^ sum(l - 1) ^ s1[r] ^ s1[l - 1];
        }
    }
    rep(i, 0, m) cout << ans[i] << '\n';
    return 0;
}

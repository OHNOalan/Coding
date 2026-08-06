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

void solve() {
    int n;
    cin >> n;
    string s;
    cin >> s;
    for (char &c: s) c -= 'a';
    auto get = [&]() -> ll {
        vector<ll> pre(n + 1), suf(n + 1);
        int pos = 0;
        rep(i, 0, n) {
            pre[i + 1] = pre[i];
            if (s[i]) {
                pre[i + 1] += (i - pos);
                pos++;
            }
        }
        pos = n - 1;
        repr(i, n - 1, -1) {
            suf[i] = suf[i + 1];
            if (s[i]) {
                suf[i] += (pos - i);
                pos--;
            }
        }
        ll ans = LLONG_MAX;
        rep(i, 0, n + 1) { ans = min(ans, pre[i] + suf[i]); }
        return ans;
    };
    ll ans = get();
    for (char &c: s) c = 1 - c;
    ans = min(ans, get());
    cout << ans << "\n";
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t = 1;
    if (true)
        cin >> t;
    while (t--) solve();
    return 0;
}

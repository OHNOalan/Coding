#include <bits/stdc++.h>

#define pb emplace_back
#define mp make_pair
#define x first
#define y second
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

typedef long double ld;
typedef long long ll;

using namespace std;

mt19937 rnd(time(nullptr));

const int inf = 1e9;
const int M = 1e9 + 7;
const ld pi = atan2(0, -1);
const ld eps = 1e-6;
int init = []() -> int { return 0; }();
void solve() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> a[i];

    vector<int> pre(n + 1), suf(n + 1);
    {
        set<int> st;
        for (int i = 1; i <= n; i++) {
            st.insert(i);
        }
        for (int i = 1; i < n; i++) {
            auto it = st.upper_bound(a[i]);
            if (it != st.begin())
                it--, st.erase(*it);
            pre[i] = n - st.size();
        }
    }
    {
        set<int> st;
        for (int i = 1; i <= n; i++) {
            st.insert(i);
        }
        for (int i = n; i > 1; i--) {
            auto it = st.upper_bound(a[i]);
            if (it != st.begin())
                it--, st.erase(*it);
            suf[i] = n - st.size();
        }
    }
    long long ans = 0;
    for (int i = 1; i < n; i++)
        ans += min(pre[i], suf[i + 1]);
    cout << ans << '\n';
}


#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
        // cout << "\n";
    }
    return 0;
}

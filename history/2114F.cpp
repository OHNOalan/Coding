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

int get_ans(int x, int k) {
    if (x == 1)
        return 0;
    vector<int> divs;
    for (int i = 1; i * i <= x; i++)
        if (x % i == 0) {
            divs.push_back(i);
            divs.push_back(x / i);
        }
    sort(all(divs));
    int n = divs.size();
    vector<int> dp(n, 100);
    dp[0] = 0;
    for (int i = 1; i < n; i++)
        for (int j = i - 1; j >= 0; j--)
            if (divs[i] % divs[j] == 0 && divs[i] / divs[j] <= k)
                dp[i] = min(dp[i], dp[j] + 1);
    return dp[n - 1] == 100 ? -1 : dp[n - 1];
}

void solve() {
    int x, y;
    int k;
    cin >> x >> y >> k;
    int g = gcd(x, y);
    int ax = get_ans(x / g, k);
    int ay = get_ans(y / g, k);
    if (ax == -1 || ay == -1)
        cout << "-1";
    else
        cout << ax + ay;
}


#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
        cout << "\n";
    }
    return 0;
}

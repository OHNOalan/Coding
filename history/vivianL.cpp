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
const int M2 = 998244353;
int init = []() -> int { return 0; }();

int getX(char c) {
    if (c == 'I')
        return 0;
    else if (isupper(c))
        return 1;
    else if (c == 'l')
        return 2;
    else if (islower(c))
        return 3;
    else if (c == '1')
        return 4;
    else if (isdigit(c))
        return 5;
    assert(c == '?');
    return 6;
}
void solve() {
    string s;
    cin >> s;
    int n = s.size();
    string specials = "Il1";
    if (specials.find(s[0]) != string::npos) {
        cout << 0;
        return;
    }
    vector f(n, vector<ll>(6));
    int fst = getX(s[0]);
    if (fst == 6) {
        f[0][1] = 25, f[0][3] = 25, f[0][5] = 9;
    } else {
        f[0][fst] = 1;
    }
    for (int i = 1; i < n; i++) {
        int x = getX(s[i]), j = i - 1;
        ll sum = accumulate(f[j].begin(), f[j].end(), 0LL);
        if (x == 6) {
            f[i][0] = f[j][0] + f[j][1];
            f[i][1] = 25LL * (sum - f[j][2] - f[j][4]) % M2;
            f[i][2] = f[j][2] + f[j][3];
            f[i][3] = 25LL * (sum - f[j][0] - f[j][4]) % M2;
            f[i][4] = f[j][4] + f[j][5];
            f[i][5] = 9LL * (sum - f[j][0] - f[j][2]) % M2;
        } else if (x == 0) {
            f[i][0] = f[j][0] + f[j][1];
        } else if (x == 1) {
            f[i][1] = (sum - f[j][2] - f[j][4]) % M2;
        } else if (x == 2) {
            f[i][2] = f[j][2] + f[j][3];
        } else if (x == 3) {
            f[i][3] = (sum - f[j][0] - f[j][4]) % M2;
        } else if (x == 4) {
            f[i][4] = f[j][4] + f[j][5];
        } else if (x == 5) {
            f[i][5] = (sum - f[j][0] - f[j][2]) % M2;
        }
    }
    ll ans = (f[n - 1][1] + f[n - 1][3] + f[n - 1][5]) % M2;
    cout << ans;
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
        cout << "\n";
    }
    return 0;
}

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
void solve() {
    int n;
    cin >> n;
    int a, b, x;
    cin >> a;
    for (int i = 0; i < n - 1; i++)
        cin >> x;
    cin >> b;
    for (int i = 0; i < n - 1; i++)
        cin >> x;
    if (a == b)
        cout << "YES";
    else
        cout << "NO";
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

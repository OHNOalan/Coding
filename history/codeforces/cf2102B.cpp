// sol1
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
    int x;
    cin >> x;
    int up = 0, down = 0;
    for (int i = 0, y; i < n - 1; i++) {
        cin >> y;
        if (y >= x)
            up++;
        if (y <= x)
            down++;
        if (-y >= x)
            up++;
        if (-y <= x)
            down++;
    }
    if (down < (n - 1) / 2 || up < (n - 1) / 2)
        cout << "NO";
    else
        cout << "YES";
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


// sol2
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
    vector<array<int, 2>> a(n);
    vector<int> ans(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i][0];
        a[i][1] = i;
        a[i][0] = abs(a[i][0]);
    }
    sort(all(a));
    for (int i = 0; i < n / 2 + 1; i++) {
        ans[a[i][1]] = 1;
    }
    if (ans[0])
        cout << "YES";
    else
        cout << "NO";
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

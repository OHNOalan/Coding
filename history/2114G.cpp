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

int max_op(int a, int b) {
    int min_part = a;
    while (min_part % 2 == 0 && min_part / 2 != b) {
        min_part /= 2;
    }
    if (min_part % 2 == 1) {
        return a / min_part;
    }
    int true_min = min_part;
    while (true_min % 2 == 0) {
        true_min /= 2;
    }
    return 1 + (a - min_part) / true_min;
}

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int &e: a)
        cin >> e;

    vector<ll> pre(n), suf(n);
    for (int i = 0; i < n - 1; i++)
        pre[i + 1] = pre[i] + max_op(a[i], a[i + 1]);
    for (int i = n - 2; i >= 0; i--) {
        suf[i] = suf[i + 1] + max_op(a[i + 1], a[i]);
    }
    for (int i = 0; i < n; i++) {
        ll res = max_op(a[i], 0) + pre[i] + suf[i];
        if (res >= k) {
            cout << "YES";
            return;
        }
    }
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

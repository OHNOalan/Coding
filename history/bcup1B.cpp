#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

int request(int l, int r) {
    cout << "? " << l << " " << r << endl;
    int res; cin >> res;
    return res;
}

void response(pair<int, int> p) {
    cout << "! " << p.first << " " << p.second << endl;
}

void solve() {
    int n; cin >> n;
    int l = 0, r = n;
    int mx = request(1, n);
    auto find_max = [&](int &l, int &r) -> void {
        while(r - l > 1) {
            int m = (l + r) / 2;
            (request(m+1, r) == mx ? l : r) = m;
        }
    };
    find_max(l, r);
    int a = r == 1 ? 0 : request(1, r-1);
    int b = r == n ? 0 : request(r+1, n);
    if(a > b) {
        l = 0, r = r-1;
        mx = a;
    } else {
        l = r, r = n;
        mx = b;
    };
    find_max(l, r);
    response({mx, r});
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

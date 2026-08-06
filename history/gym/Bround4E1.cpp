#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int cal(int l, int r) {
    int n = r - l - 1;
    return (n + 1) / 2;
}

void solve() {
    int n, q; cin >> n >> q;
    set<int> on_seat;
    on_seat.insert(0);
    on_seat.insert(n+1);
    int num = cal(0, n+1);
    for(int i = 0; i < q; i++) {
        int k; cin >> k;
        auto p = on_seat.find(k);
        if(p != on_seat.end()) {
            int l = *prev(p);
            int r = *next(p);
            on_seat.erase(p);
            num += cal(l, r);
            num -= cal(l, k);
            num -= cal(k, r);
        } else {
            p = on_seat.insert(k).first;
            int l = *prev(p);
            int r = *next(p);
            num -= cal(l, r);
            num += cal(l, k);
            num += cal(k, r);
        }
        cout << num << '\n';
    }
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
    }
    return 0;
}
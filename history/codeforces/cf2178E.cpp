#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

void solve() {
    int n;
    cin >> n;
    // (st,ed) (l,r)
    int st = 0, ed = n + 1;
    auto ask = [](int l, int r) -> i64 {
        i64 ans;
        cout << "? " << l + 1 << " " << r - 1 << endl;
        cin >> ans;
        return ans;
    };
    i64 sum = ask(st, ed);
    while (ed - st - 1 > 1) {
        assert((sum & 1) == 0);
        // (st, st + 2) (st + 1, ed)
        // (st, ed - 1) (ed - 2, ed)
        int l = st + 1, r = ed;
        // (l, r)
        while (l + 1 < r) {
            int m = (l + r) / 2;
            if (ask(st, m) <= sum / 2)
                l = m;
            else
                r = m;
        }
        // (st, l) <= sum / 2 (l-1, ed)
        assert(l - st - 1 > 0);
        assert(ed - (l - 1) - 1 > 0);
        if (l - st <= ed - (l - 1))
            ed = l;
        else
            st = l - 1;
        sum >>= 1;
    }
    assert(ed - st - 1 == 1);
    i64 ans = ask(st, ed);
    cout << "! " << ans << endl;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

int ask(int i, int j) {
    cout << "? " << i << " " << j << endl;
    int ans;
    cin >> ans;
    assert(ans != -1);
    return ans;
}

void response(int i) { cout << "! " << i << endl; }

void solve() {
    int n;
    cin >> n;
    for (int i = 3; i < 2 * n; i += 2) {
        if (ask(i, i + 1)) {
            response(i);
            return;
        }
    }
    if (ask(1, 3) || ask(1, 4)) {
        response(1);
    } else {
        response(2);
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

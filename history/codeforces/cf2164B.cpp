#include <bits/stdc++.h>

using namespace std;

void solve1() {
    int n, x;
    cin >> n;
    vector<int> odds, evens;
    for (int i = 0; i < n; i++) {
        cin >> x;
        (x % 2 ? odds : evens).push_back(x);
    }
    if (evens.size() >= 2) {
        cout << evens[0] << " " << evens[1] << '\n';
        return;
    }
    if (evens.size() == 1) {
        int y = evens[0];
        for (int x: odds) {
            if (x > y) break;
            if ((y % x) % 2 == 0) {
                cout << x << " " << y << '\n';
                return;
            }
        }
    }
    for (int i = 0; i < odds.size() - 1; i++) {
        if ((odds[i + 1] % odds[i]) % 2 == 0) {
            cout << odds[i] << " " << odds[i + 1] << '\n';
            return;
        }
    }

    for (int i = 0; i < odds.size() - 1; i++) {
        for (int j = i + 1; j < odds.size(); j++) {
            if ((odds[j] % odds[i]) % 2 == 0) {
                cout << odds[i] << " " << odds[j] << "\n";
                return;
            }
        }
    }
    cout << "-1\n";
}

void solve() {
    int n, x;
    cin >> n;
    vector<int> a(n);
    for (int &x: a) cin >> x;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if ((a[j] % a[i]) % 2 == 0) {
                cout << a[i] << " " << a[j] << '\n';
                return;
            }
    cout << "-1\n";
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

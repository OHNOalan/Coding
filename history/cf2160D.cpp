#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

int query(vector<int> &vec) {
    cout << "? ";
    cout << vec.size() << " ";
    for (int x: vec) cout << x << " ";
    cout << endl;
    int x;
    cin >> x;
    return x;
}

void solve() {
    int n;
    cin >> n;

    vector<int> arr(2 * n + 1);
    vector<int> idx{1};
    for (int i = 2; i <= 2 * n; i++) {
        idx.push_back(i);
        int ans = query(idx);
        if (ans != 0) {
            arr[i] = ans;
            idx.pop_back();
        }
    }
    idx = {2 * n};
    for (int i = 2 * n - 1; i >= 1; i--) {
        idx.push_back(i);
        int ans = query(idx);
        if (ans != 0) {
            arr[i] = ans;
            idx.pop_back();
        }
    }
    cout << "! ";
    for (int i = 1; i <= 2 * n; i++) cout << arr[i] << " ";
    cout << endl;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

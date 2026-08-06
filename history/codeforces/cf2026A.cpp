#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
static constexpr int MOD = 1e9 + 7;

void solve() {
    int x,y,k;
    cin >> x >> y >> k;
    int d = min(x, y);
    vector<int> line1, line2;
    if(d >= k) {
        // cout << "A\n";
        line1.push_back(0);
        line1.push_back(0);
        line1.push_back(0);
        line1.push_back(d);
        line2.push_back(0);
        line2.push_back(0);
        line2.push_back(d);
        line2.push_back(0);
    } else {
        // cout << "B\n";
        int a2 = k * k - d * d;
        int a = sqrt(a2 - 1) + 1;
        line1.push_back(0);
        line1.push_back(a);
        line1.push_back(d);
        line1.push_back(0);
        line2.push_back(0);
        line2.push_back(0);
        line2.push_back(a);
        line2.push_back(d);
    }
    for(int x : line1)
        cout << x << ' ';
    cout << '\n';
    for(int x : line2)
        cout << x << ' ';
    cout << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
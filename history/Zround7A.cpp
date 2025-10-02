#include <bits/stdc++.h>
using namespace std;


using ll = long long;

char LET[4] = {'S', 'H', 'D', 'C'};

void solve() {
    int s, h, d, c; cin >> s >> h >> d >> c;
    vector<pair<int, int>> vec(4);
    vec[0] = {-1 * s, 0};
    vec[1] = {-1 * h, 1};
    vec[2] = {-1 * d, 2};
    vec[3] = {-1 * c, 3};
    ranges::sort(vec);
    if(vec[0].first >= -7) {
        cout << "N\n";
    } else {
        cout << LET[vec[0].second] << '\n';
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

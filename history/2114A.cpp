#include <bits/stdc++.h>
using namespace std;

const int MX = 1e5;
int SQR[MX];

int init = []() -> int {
    memset(SQR, 0xff, sizeof(SQR));
    for (int i = 0; i * i < MX; i++) {
        SQR[i * i] = i;
    }
    return 0;
}();

void solve() {
    string s;
    cin >> s;
    int x = stoi(s);
    if (SQR[x] != -1)
        cout << 0 << " ";
    cout << SQR[x] << endl;
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
    }
    return 0;
}

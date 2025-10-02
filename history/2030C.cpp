#include <bits/stdc++.h>
using namespace std;

using ll = long long;
static constexpr int mod = 1e9 + 7;

void solve() {
    int n; cin >> n;
    string s; cin >> s;
    s = '1' + s + '1';
    if(s.find("11") != -1) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
	int t; cin >> t;
	while(t--) {
		solve();
	}
    return 0;
}
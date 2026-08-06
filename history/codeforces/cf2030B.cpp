#include <bits/stdc++.h>
using namespace std;

using ll = long long;
static constexpr int mod = 1e9 + 7;

void solve() {
    int n; cin >> n;
    string s(n, '0');
    for(int i=1; i<n; i+=2)
        s[i] = '1';
    cout << s << endl;
}

int main() {
	int t; cin >> t;
	while(t--) {
		solve();
	}
    return 0;
}
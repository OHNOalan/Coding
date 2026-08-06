#include <bits/stdc++.h>
using namespace std;

using ll = long long;
static constexpr int mod = 1e9 + 7;

void solve() {
	int n, x; cin >> n;
	int mn = INT_MAX, mx = 0;
	for(int i=0; i<n; i++) {
		cin >> x;
		mn = min(mn, x);
		mx = max(mx, x);
	}
	cout << (mx - mn) * (n-1) << endl;
}

int main() {
	int t; cin >> t;
	while(t--) {
		solve();
	}
    return 0;
}
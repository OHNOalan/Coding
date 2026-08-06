#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int,int>;
static constexpr int MOD = 1e9 + 7;
static constexpr ll INF = 1e18 + 1;

void solve() {
    int n; cin >> n;
    string s; cin >> s;
    ll sum = (ll) n * (n + 1) / 2;

    stack<int> zeros, ones;
    for (int i = 0; i < n; i++)
        if(s[i] == '0')
            zeros.push(i);

    for (int i = n-1; i >=0; i--)
        if(s[i] == '1')
            ones.push(i);
    int i=n-1;
    while(i >= 0) {
        if(s[i] == '1') {
            while(!zeros.empty() && zeros.top() > i) {
                zeros.pop();
            }
            if(!zeros.empty()) {
                sum -= i + 1;
                zeros.pop();
            } else if(ones.top() < i) {
                sum -= i + 1;
                ones.pop();
            } else {
                break;
            }
        }
        i--;
    }
    cout << sum << '\n';
}

int main() {
    int t; cin >> t;
    while(t--) {
        solve();
    }
    return 0;
}
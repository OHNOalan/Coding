#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n; ll k; cin >> n >> k;
    string s; cin >> s;
    priority_queue<int> pq;
    for(int i=n-1, sum = 0; i>0; i--) {
        int x = s[i] - '0';
        sum += (x * 2) - 1;
        pq.push(sum);
    }
    int ans = -1;
    ll sum = 0;
    while(pq.size()) {
        sum += pq.top(); pq.pop();
        if(sum >= k) {
            ans = n - pq.size();
            break;
        }
    }
    cout << ans << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
     freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
//    while (t--) {
//        solve();
//    }
    cout << "1" << endl;
    cout << string(2e5, '0') << endl;
    return 0;
}

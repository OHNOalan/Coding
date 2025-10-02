#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 1e7 + 10;

using ll = long long;

void solve() {
    string s, t, name1, name2;
    ll x;
    getline(cin, s);
    map<string, ll> mp;
    while(getline(cin, s)) {
        stringstream ss(s);
        ss >> t;
        if(t == "}") return;
        if(t == "var") {
            ss >> name1;
            ss >> t;
            ss >> t;
            ss >> x;
            mp[name1] = x;
        } else if(t.substr(0, 3) == "fmt") {
            name1 = t.substr(12, t.size() - 12 - 1);
            cout << mp[name1] << endl;
        } else {
            name1 = t;
            ss >> t;
            ss >> name2;
            mp[name1] += mp[name2];
        }
    }
}

#define MULTICASE false

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

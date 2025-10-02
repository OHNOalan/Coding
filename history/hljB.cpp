#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

const int MOD = 998244353;

#define DEBUG false

template<typename T>
void print(vector<T> &a, function<T(T x)> f = [](T x) { return x; }) {
    for(int i=0; i<a.size(); i++) {
        cout << f(a[i]) << " ";
    }
    cout << endl;
}

void print(set<int>& st) {
    for(int x : st) cout << x << " ";
    cout << endl;
}

void print(map<int, int>& mp) {
    for(auto [k, v] : mp) cout << k << " " << v << endl;
    cout << endl;
}

// vector<int> res;

void solve() {
    int n, q; cin >> n >> q;
    string s; cin >> s;
    char c; int l, r;
    while(q--) {
        cin >> c >> l >> r;
        if(c == 'Q') {

        } else if(c == 'M') {

        }
    }
}

void check(int i) {
    // TODO
}

#define MULTICASE false

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    for(int i=0; i<t; i++) {
        solve();
    }
    // for(int i=0; i<t; i++) {
    //     check(i);
    // }
    return 0;
}

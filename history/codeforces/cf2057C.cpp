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

vector<int> res;

int f(int a, int b, int c) {
    return (a ^ b) + (b ^ c) + (a ^ c);
}

void solve() {
    int l, r; cin >> l >> r;
    bool diff = false;
    int a = 0, b = 0, c = 0;
    int last = 31;
    int st = 30;
    while(!(r>>st&1)) st--;
    int first = 31;
    for(int i=st; i>=0; i--) {
        if(diff) {
            b |= 1<<i;
            c |= 1<<i;
            if((l>>i&1) == 0) last = i;
        } else if((l>>i&1) != (r>>i&1)) {
            a |= 1<<i;
            diff = true;
            first = i;
            last = i;
        } else {
            int d = l>>i&1;
            a |= d<<i;
            b |= d<<i;
            c |= d<<i;
        }
        // cout << a << " " << b << " " << c << "\n";
    }
    if(last != 31) {
        if(last != first) {
            b -= 1<<last;
        } else {
            b += 2;
        }
    }
    cout << a << " " << b << " " << c << "\n";
    res.push_back(f(a, b, c));
}

void check(int i) {
    int a, b, c; cin >> a >> b >> c;
    cout << a << " " << b << " " << c << "\n";
    assert(f(a, b, c) == res[i]);
}

#define MULTICASE true

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
    for(int i=0; i<t; i++) {
        check(i);
    }
    return 0;
}

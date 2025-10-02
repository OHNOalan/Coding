#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void print(int a) {
    cout << a << endl;
}
void print(vector<int> &a) {
    for(int i=0; i<a.size(); i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}
void print(map<int, int>& mp) {
    for(auto [k, v] : mp) cout << k << " " << v << endl;
    cout << endl;
}

void YESNO(bool flag) {
    if(flag) cout << "YES\n";
    else cout << "NO\n";
}

void solve() {
    int n; cin >> n;
    vector<int> a(2 * n);
    for(int i = 0; i < 2 * n; i++) cin >> a[i];
    map<int, int> cnt;
    for(int i=0, pre=0; i<=2*n; i++) {
        if(i == 2 * n || a[i] > a[pre]) {
            if(i - pre > n) {
                YESNO(false);
                return;
            }
            cnt[i - pre]++;
            pre = i;
        }
    }
    vector<int> xs;
    for(auto [x, t] : cnt) {
        int c = 1;
        while (t > c) {
            t -= c;
            xs.push_back(c * x);
            c *= 2;
        }
        xs.push_back(t * x);
    }
    bitset<1000001> f;
    f.set(0);
    for(int x : xs) {
        f |= f << x;
    }
    YESNO(f[n]);
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
    while (t--) {
        solve();
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void init() {}

int query(int a, int b) {
    cout << "? " << a << " " << b << endl;
    int res;
    cin >> res;

    if (res == -1) {
        exit(0);
    }
    return res;
}

void solve() {
    int n;
    cin >> n;

    int a = 2;

    vector<int> p(n);
    while(query(a, 1)) {
        a++;
    }
    p[a] = 1;
    int l = 2;
    for(int i=a+1; i<n; i++) {
        while(query(i, l)) {
            l++;
        }
        p[i] = l++;
    }
    cout << "!";
    for(int i=1; i<n; i++) {
        cout << " " << p[i];
    }
    cout << endl;
}

#define MULTICASE true
int main() {
    init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
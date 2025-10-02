#include <bits/stdc++.h>
using namespace std;

using ll = long long;

char query(int m, vector<int>& a, vector<int>& b) {
    cout << "? " << m << " ";
    for(int x : a) cout << x << " ";
    for(int x : b) cout << x << " ";
    cout << endl;
    char res; cin >> res;
    return res;
}

void response(int x) {
    cout << "! " << x << endl;
}

void solve() {
    int n; cin >> n;
    vector<int> tot(n);
    iota(tot.begin(), tot.end(), 1);
    while(tot.size() > 1) {
        int m = (tot.size() + 1) / 3;
        vector<int> a, b;
        a.reserve(m);
        b.reserve(m);
        for(int i=0; i<m; i++) {
            a.push_back(tot[i]);
            b.push_back(tot[i+m]);
        }
        char res = query(m, a, b);
        vector<int> ntot;
        switch(res) {
            case '\\':
                ntot.reserve(m);
            for(int i=0; i<m; i++) {
                ntot.push_back(tot[i]);
            }
            break;
            case '/':
                ntot.reserve(m);
            for(int i=m; i<2*m; i++) {
                ntot.push_back(tot[i]);
            }
            break;
            default:
                ntot.reserve(tot.size() - 2 * m);
            for(int i=2*m; i<tot.size(); i++) {
                ntot.push_back(tot[i]);
            }
        }
        tot = std::move(ntot);
    }
    response(tot[0]);
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
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

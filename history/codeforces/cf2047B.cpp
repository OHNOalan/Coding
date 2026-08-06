#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

void solve() {
    int n; cin >> n;
    string s; cin >> s;
    map<char, int> cnt;
    for(char c : s) {
        cnt[c]++;
    }
    set<pair<int, char>> st;
    for(auto [k, v] : cnt) {
        st.insert({v, k});
    }

//    for(auto [k, v] : cnt) cout << v << " " << k << endl;
    // map 不行
    auto first = st.begin();
    auto last = st.rbegin();
    {
        char a = first->second, b = last->second;
//        cout << a << ' ' << b << endl;
//        cout << first->first << ' ' << last->first << endl;
        for(char &c : s) {
            if(c == a) {
                c = b;
                break;
            }
        }
    }

    cout << s << '\n';
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

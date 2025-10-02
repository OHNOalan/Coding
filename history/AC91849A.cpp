#include <bits/stdc++.h>
using namespace std;

using ll = long long;

void solve() {
    int n; cin >> n;
    string s; cin >> s;
    int human = 0, bot = 1;
    for(char c : s) {
        if(c == '0')  tie(human, bot) = make_pair(min(human,bot), bot + 1);
        else tie(human, bot) = make_pair(bot, min(human,bot) + 1);
    }
    cout << min(human, bot) << '\n';
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

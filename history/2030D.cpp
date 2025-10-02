#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> p(n);
    for (int i = 0; i < n; i++)
        cin >> p[i];

    vector<int> need(n);

    for (int i = 0; i < n; i++) {
        int x = i, px = p[i] - 1;
        if (x > px)
            swap(x, px);
        need[x]++, need[px]--;
    }

    int tot = 0;
    for (int i = 0; i < n - 1; i++) {
        tot += need[i] > 0;
        need[i + 1] += need[i];
    }

    vector<int> cnt(n - 1);
    string s;
    cin >> s;
    for (int i = 0; i < n; i++) {
        if (s[i] == 'L')
            cnt[i - 1]++;
        if (s[i] == 'R')
            cnt[i]++;
    }

    int own = 0;
    for (int i = 0; i < n - 1; i++)
        if (need[i] && cnt[i])
            own++;

    for (int i = 0; i < q; i++) {
        int x;
        cin >> x;
        x--;
        if (s[x] == 'R') {
            int inc = need[x - 1] && cnt[x - 1] == 0;
            cnt[x]--, cnt[x - 1]++;
            int dec = need[x] && cnt[x] == 0;
            own = own - dec + inc;
            s[x] = 'L';
        } else if (s[x] == 'L') {
            int inc = need[x] && cnt[x] == 0;
            cnt[x - 1]--, cnt[x]++;
            int dec = need[x - 1] && cnt[x - 1] == 0;
            own = own - dec + inc;
            s[x] = 'R';
        }
        cout << (tot == own ? "YES" : "NO") << endl;
    }
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
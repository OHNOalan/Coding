#include <bits/stdc++.h>

using namespace std;

struct Node {
    bool valid = true;
    int prev = -1;
    int next = -1;
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;
    s.insert(s.begin(), ' ');

    vector<Node> ns(n + 2);
    ns[0].valid = ns[n + 1].valid = false;
    ns[0].next = 1;
    ns[n + 1].prev = n;
    for (int i = 1; i <= n; i++) {
        ns[i].prev = i - 1;
        ns[i].next = i + 1;
    }

    auto prev = [&](int i) { return ns[i].prev; };
    auto next = [&](int i) { return ns[i].next; };
    auto valid = [&](int i) { return ns[i].valid; };

    auto get = [&](int i, int len) -> string {
        string t;
        while (len--) {
            if (!valid(i)) return "";
            t += s[i];
            i = next(i);
        }
        return t;
    };

    auto getIdxs = [&](int i, int len) -> vector<int> {
        vector<int> ans;
        while (len--) {
            if (!valid(i)) return {};
            ans.push_back(i);
            i = next(i);
        }
        return ans;
    };

    unordered_map<string, set<int>> mp;
    for (int i = 1; i <= n; i++) {
        for (int len = 1; len <= 5; len++) {
            string t = get(i, len);
            if (!t.empty()) mp[t].insert(i);
        }
    }

    auto erase = [&](int i) -> void {
        // assert(valid(i));
        vector<int> xs;
        {
            int x = i;
            for (int r = 0; r < 5; r++) {
                xs.push_back(x);
                int y = x;
                string t;
                for (int j = 1; j <= 5; j++) {
                    t += s[y];
                    if (y >= i) {
                        // assert(mp[t].contains(x));
                        mp[t].erase(x);
                        if (mp[t].empty()) mp.erase(t);
                    }
                    y = next(y);
                    if (!valid(y)) break;
                }
                x = prev(x);
                if (!valid(x)) break;
            }
        }

        ns[i].valid = false;
        ns[next(i)].prev = prev(i);
        ns[prev(i)].next = next(i);

        // add map
        xs.erase(xs.begin());
        for (int x: xs) {
            int y = x;
            string t;
            for (int j = 1; j <= 5; j++) {
                t += s[y];
                mp[t].insert(x);
                y = next(y);
                if (!valid(y)) break;
            }
        }
    };

    while (q--) {
        int m;
        cin >> m;
        string t;
        cin >> t;
        vector<int> ids;
        for (int i: mp[t]) {
            if (ids.empty()) {
                ids.push_back(i);
                continue;
            }
            int last = ids.back();
            for (int j = 1; j < t.size(); j++) {
                last = next(last);
                if (last == i) break;
            }
            if (last == i) continue;
            ids.push_back(i);
        }
        cout << ids.size() << '\n';
        for (int i: ids) {
            for (int j: getIdxs(i, t.size())) {
                erase(j);
            }
        }
    }
}

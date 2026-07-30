#include <bits/stdc++.h>

using namespace std;

string query(const vector<int> &a) {
    cout << "? " << a.size();
    for (auto v: a) cout << " " << v + 1;
    cout << endl;
    string s;
    cin >> s;
    return s;
}

vector<int> work(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    vector<int> s;
    for (auto v: a) s.push_back(v);
    for (auto v: b) s.push_back(v);
    string t = query(s);
    vector<int> r;
    for (int i = 0; i < b.size(); i++)
        if (t[i + a.size()] == '0') r.push_back(b[i]);
    return r;
}

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> g(n);

    auto find = [&](this auto &&self, const vector<int> &a, const vector<int> &b) -> void {
        if (a.empty() && b.empty()) return;
        if (a.size() == 1) {
            int x = a[0];
            for (auto y: b) g[x].push_back(y), g[y].push_back(x);
            return;
        }
        int m = a.size() / 2;
        vector a1(a.begin(), a.begin() + m);
        vector a2(a.begin() + m, a.end());
        auto p1 = work(a1, b);
        set<int> vis(p1.begin(), p1.end());
        vector<int> nb, p2;
        for (auto v: b) (vis.count(v) ? nb : p2).push_back(v);
        self(a1, p1);
        for (auto v: work(a2, nb)) p2.push_back(v);
        self(a2, p2);
    };

    vector<int> a(n);
    ranges::iota(a, 0);
    [&](this auto &&self, const vector<int> &a) -> void {
        if (a.size() <= 1) return;
        string t = query(a);
        vector<int> p1, p2;
        for (int i = 0; i < a.size(); i++) (t[i] == '0' ? p2 : p1).push_back(a[i]);
        self(p2);
        vector<int> col(n, -1);
        vector<vector<int>> b(2);
        for (auto w: p2) {
            if (col[w] == -1) {
                queue<int> q;
                q.push(w);
                col[w] = 0;
                while (!q.empty()) {
                    int x = q.front();
                    q.pop();
                    b[col[x]].push_back(x);
                    for (auto y: g[x]) {
                        if (col[y] == -1) {
                            col[y] = col[x] ^ 1;
                            q.push(y);
                        } else {
                            assert(col[y] == (col[x] ^ 1));
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 2; i++) find(b[i], work(b[i], p1));
    }(a);

    cout << "!" << endl;
    for (int x = 0; x < n; x++)
        for (auto y: g[x])
            if (y > x) cout << x + 1 << " " << y + 1 << endl;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

#include <bits/stdc++.h>

using namespace std;

constexpr int P = 110;
constexpr int N = 1e6;
constexpr int B1 = 20;
constexpr int B2 = 18;
constexpr int sz_v = 1 << B2;

vector<int> ps, pks, v;
array<int, N + 1> idx;

void init() {
    vector<int> max_p(N + 1);
    max_p[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (max_p[i] == 0) {
            ps.push_back(i);
            for (int j = i; j <= N; j += i) {
                max_p[j] = i;
            }
        }
    }
    ps.resize(P);
    for (int p: ps) {
        int x = p;
        while (x * p <= N) x *= p;
        pks.push_back(x);
    }
    for (int i = 1; i <= N && v.size() < sz_v; i++) {
        if (max_p[i] <= ps.back()) {
            v.push_back(i);
        }
    }
    assert(size(v) == sz_v);
    for (int i = 0; i < sz_v; i++) idx[v[i]] = i;
}

void run1() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    vector<int> b;
    for (int pk: pks) b.push_back(pk);
    vector<int> bits;
    for (int i = 0; i < n; i++) {
        for (int j = B1 - 1; j >= 0; j--) {
            bits.push_back(a[i] >> j & 1);
        }
    }
    while (bits.size() % B2) bits.push_back(0);
    for (int i = 0; i < bits.size(); i += B2) {
        int now = 0;
        for (int j = 0; j < B2; j++) {
            now = (now << 1) | bits[i + j];
        }
        b.push_back(v[now]);
    }
    int k = b.size();
    cout << k << '\n';
    for (int i = 0; i < k; i++) cout << b[i] << ' ';
    cout << '\n';
}

void run2() {
    int n, k;
    cin >> n >> k;
    auto ask = [&](int i, int j) -> int {
        assert(i != j);
        cout << "? " << i + 1 << ' ' << j + 1 << endl;
        int g;
        cin >> g;
        if (g == -1) exit(0);
        return g;
    };
    vector<int> bits;
    for (int i = P; i < k; i++) {
        int x = 1;
        for (int j = 0; j < P; j++) {
            int g = ask(i, j);
            x *= g;
        }
        int now = idx[x];
        for (int j = B2 - 1; j >= 0; j--) {
            bits.push_back(now >> j & 1);
        }
    }
    vector<int> a(n);
    for (int i = 0, j = 0; i < n; i++, j += B1) {
        int now = 0;
        for (int x = 0; x < B1; x++) {
            now = (now << 1) | bits[j + x];
        }
        a[i] = now;
    }
    cout << "!";
    for (int i = 0; i < n; i++) cout << ' ' << a[i];
    cout << endl;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();

    string type;
    cin >> type;
    int t = 1;
    cin >> t;
    while (t--) {
        if (type == "first") {
            run1();
        } else {
            run2();
        }
    }
}

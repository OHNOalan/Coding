#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct Line {
    ll a, b, c;
    // 优化：内联比较函数，加快排序速度
    bool operator<(const Line& other) const {
        if (a != other.a) return a < other.a;
        if (b != other.b) return b < other.b;
        return c < other.c;
    }
    bool operator==(const Line& other) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

// 快速 GCD
ll fast_gcd(ll a, ll b) {
    while (b) { a %= b; swap(a, b); }
    return a;
}

void solve() {
    int n;
    double p_percent;
    if (!(cin >> n >> p_percent)) return;

    vector<pair<ll, ll>> ps(n);
    for (int i = 0; i < n; i++) {
        cin >> ps[i].first >> ps[i].second;
    }

    if (n == 0) { cout << "NO\n"; return; }

    vector<Line> lines;
    lines.reserve(n * (n - 1) / 2);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            ll x1 = ps[i].first, y1 = ps[i].second;
            ll x2 = ps[j].first, y2 = ps[j].second;

            ll A = 2 * (x2 - x1);
            ll B = 2 * (y2 - y1);
            ll C = -(x2 * x2 - x1 * x1 + y2 * y2 - y1 * y1);

            ll common = fast_gcd(abs(A), fast_gcd(abs(B), abs(C)));
            A /= common; B /= common; C /= common;
            if (A < 0 || (A == 0 && B < 0) || (A == 0 && B == 0 && C < 0)) {
                A = -A; B = -B; C = -C;
            }
            lines.push_back({A, B, C});
        }
    }

    // 排序比 map 快得多
    sort(lines.begin(), lines.end());

    int max_powerful = 0;
    int current_cnt = 0;
    for (size_t i = 0; i < lines.size(); i++) {
        if (i > 0 && lines[i] == lines[i - 1]) {
            current_cnt++;
        } else {
            current_cnt = 1;
        }
        max_powerful = max(max_powerful, current_cnt);
    }

    if ((double)max_powerful * 2.0 / n * 100.0 >= p_percent - 1e-9) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
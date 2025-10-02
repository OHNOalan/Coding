#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int MX = 1e5 + 1;

vector<int> comp(MX), ps;
vector<vector<int>> factors(MX);

int init = []() -> int {
    comp[0] = comp[1] = 1;
    for (int i = 2; i < MX; i++)
        if (!comp[i]) {
            ps.push_back(i);
            for (int j = i * 2; j < MX; j += i) comp[j] = 1;
        }
    return 0;
}();

int gcd(int a, int b) {
    while (true) {
        if (a < b)
            swap(a, b);
        if (b == 0)
            return a;
        a -= (a / b) * b;
    }
}

i64 eulerPhi(i64 m) {
    long long result = m;
    for (int p: ps) {
        if (p > m)
            break;
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1)
        result -= result / m;
    return result;
}


// https://codeforces.com/group/BJlsDCvlJO/contest/558380/problem/D
void solve() {
    i64 a, m;
    cin >> a >> m;
    int d = gcd(a, m);
    m /= d;
    cout << eulerPhi(m) << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int t = 1;
    if (true)
        cin >> t;
    while (t--) solve();
}

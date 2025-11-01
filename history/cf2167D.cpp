#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int N = 55;
int comp[N];
vector<int> primes;

void init() {
    for (int i = 2; i < N; i++)
        if (!comp[i]) {
            primes.push_back(i);
            for (int j = i * i; j < N; j += i) { comp[j] = true; }
        }
}

void solve() {
    int n;
    cin >> n;
    vector<i64> a(n);
    for (i64 &x: a) cin >> x;
    for (int p: primes) {
        bool work = false;
        for (i64 x: a)
            if (x % p != 0) {
                work = true;
                break;
            }
        if (work) {
            cout << p << "\n";
            return;
        }
    }
    assert(false);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    init();

    int t;
    cin >> t;
    while (t--) solve();
}

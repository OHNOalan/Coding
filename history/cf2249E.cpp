#include <bit>
#include <bits/stdc++.h>

using namespace std;

void solve() {
    int k, q;
    cin >> k >> q;

    auto f = [&](long long x, int t) -> long long {
        if (x < 0) return 0;
        long long m = x + 1;
        long long blocks = m / k;
        int rem = m % k;

        int block_sum = 0;
        unsigned long long temp = blocks;
        if (k == 2) {
            block_sum = popcount(temp) & 1;
        } else {
            while (temp > 0) {
                block_sum = (block_sum + temp % k) % k;
                temp /= k;
            }
        }
        int required_c = (t - block_sum + k) % k;
        return blocks + (rem > required_c);
    };

    while (q--) {
        long long L, R;
        int n;
        string s;

        cin >> L >> R >> n >> s;

        if (R - L + 1 < n) {
            cout << 0 << '\n';
            continue;
        }

        vector<int> t(n);
        for (int i = 0; i < n; i++) {
            char c = s[i];
            t[i] = isdigit(c) ? c - '0' : (isupper(c) ? c - 'A' + 10 : c - 'a' + 36);
        }

        auto self = [&](this auto &&self, long long l, long long r, int n, const vector<int> &t) -> long long {
            if (l > r) return 0;
            if (n == 1) {
                return f(r, t[0]) - f(l - 1, t[0]);
            }
            int pos = 0;
            for (int i = 0; i < n - 1; i++) {
                if ((t[i] + 1) % k != t[i + 1]) {
                    pos = i + 1;
                    break;
                }
            }
            vector<int> possible_j;
            if (pos) {
                possible_j.push_back((k - (pos % k)) % k);
            } else {
                for (int j = 0; j < k; j++) possible_j.push_back(j);
            }
            long long res = 0;
            for (int j: possible_j) {
                int new_n = (j + n - 1) / k + 1;
                vector<int> np(new_n, -1);
                bool ok = true;
                for (int i = 0; i < n; i++) {
                    int block = (i + j) / k;
                    int offset = (i + j) % k;
                    int expected_val = (t[i] - offset + k) % k;

                    if (np[block] == -1) {
                        np[block] = expected_val;
                    } else if (np[block] != expected_val) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    long long nl = (l >= j) ? (l - j + k - 1) / k : 0;
                    long long nr = (r - j - n + 1) >= 0 ? (r - j - n + 1) / k : -1;
                    if (nl <= nr) {
                        res += self(nl, nr + new_n - 1, new_n, np);
                    }
                }
            }
            return res;
        };
        cout << self(L, R, n, t) << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
#ifdef LOCAL
    cin >> t;
#endif
    while (t--) solve();
}

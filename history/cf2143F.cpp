#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

const int B = 21;

struct XorBasis {
    std::vector<int> basis;
    std::vector<int> who;

    int sz;

    void reset() {
        basis.assign(B, 0);
        who.clear();
        sz = 0;
    }

    bool add(int x, int index) {
        for (int i = B - 1; i >= 0; i--) {
            if (x >> i & 1) {
                if (!basis[i]) {
                    basis[i] = x;
                    who.push_back(index);
                    sz++;
                    return true;
                }
                x ^= basis[i];
            }
        }
        return false;
    }

    int kth(int k) {
        if (k < 0 || k > (1 << sz)) {
            return -1;
        }
        int x = 0;
        int cnt = 1 << sz;
        for (int i = B - 1; i >= 0; i--) {
            if (basis[i]) {
                if (k > cnt / 2) {
                    if (!(x >> i & 1)) {
                        x ^= basis[i];
                    }
                    k -= cnt / 2;
                } else {
                    if (x >> i & 1) {
                        x ^= basis[i];
                    }
                }
                cnt /= 2;
            }
        }
        return x;
    }

    int count_le(int x) {
        if (x < 0) {
            return 0;
        }
        int ret = 0;
        int cnt = 1 << sz;
        int mask = 0;
        for (int i = B - 1; i >= 0; i--) {
            if (basis[i]) {
                if (x >> i & 1) {
                    ret += cnt / 2;
                    if (!(mask >> i & 1)) {
                        mask ^= basis[i];
                    }
                } else {
                    if (mask >> i & 1) {
                        mask ^= basis[i];
                    }
                }
                cnt /= 2;
            } else {
                if ((x ^ mask) >> i & 1) {
                    if (x >> i & 1) {
                        return ret + cnt;
                    } else {
                        return ret;
                    }
                }
            }
        }
        return ret;
    }

    int count_leq(int x) { // <= x
        return count_le(x + 1);
    }
    int jumpK(int value, int k) {
        int p = count_leq(value);
        return kth(p + k);
    }
};


void solve() {
    int n, q;
    cin >> n >> q;
    vector<i64> a(n);
    for (auto &x: a) cin >> x;

    XorBasis B;
    B.reset();

    vector<int> maxR(n);

    for (int l = n - 1; l >= 0; l--) {
        if (!B.add(a[l], l)) {
            vector<int> ids = B.who;
            ids.push_back(l);
            sort(ids.begin(), ids.end());
            B.reset();
            for (int index: ids) {
                B.add(a[index], index);
            }
            assert(is_sorted(B.who.begin(), B.who.end()));
        } else {
            sort(B.who.begin(), B.who.end());
        }

        XorBasis aux;
        aux.reset();
        swap(B, aux);

        int r = n - 1;

        int maxExtend = l;
        int prevIndex = l - 1;
        int prevValue = -1;

        for (int index: aux.who) {
            if (index - 1 > maxExtend) {
                r = min(r, maxExtend);
                break;
            }
            int before = prevValue == -1 ? -1 : B.jumpK(prevValue, index - 1 - prevIndex);
            B.add(a[index], index);
            int me = B.jumpK(before, 1);
            if (me == -1) {
                r = min(r, index - 1);
                break;
            }
            maxExtend = index + (1 << B.sz) - B.count_leq(me);
            prevValue = me;
            prevIndex = index;
        }
        r = min(r, maxExtend);
        swap(aux, B);
        maxR[l] = r;
    }

    while (q--) {
        int l, r;
        cin >> l >> r;
        l--, r--;
        if (r <= maxR[l])
            cout << "YES\n";
        else
            cout << "NO\n";
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) solve();
}

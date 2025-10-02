#include <bits/stdc++.h>
using namespace std;

struct SegmentTree01 {
    struct Node {
        int l, r, ones = 0;
        int8_t state = -1; // -1: all 0, 1: all 1, 0: mix of 0 and 1
        bool flip = false;
    };

    std::vector<Node> tree;

    SegmentTree01(int n) {
        tree.resize(2 << (32 - __builtin_clz(n - 1)));
    }

    void maintain(int o) {
        Node& lo = tree[o << 1];
        Node& ro = tree[o << 1 | 1];
        if (lo.state < 0 && ro.state < 0) {
            tree[o].state = -1;
        } else if (lo.state > 0 && ro.state > 0) {
            tree[o].state = 1;
        } else {
            tree[o].state = 0;
        }
        tree[o].ones = lo.ones + ro.ones;
    }

    void doFlip(int O) {
        Node& o = tree[O];
        o.state = -o.state;
        o.ones = o.r - o.l + 1 - o.ones;
        o.flip = !o.flip;
    }

    void spread(int o) {
        if (tree[o].flip) {
            doFlip(o << 1);
            doFlip(o << 1 | 1);
            tree[o].flip = false;
        }
    }

    // Initializes the tree based on a binary string
    void buildWithBinary(const std::string& a, int o, int l, int r) {
        tree[o].l = l;
        tree[o].r = r;
        if (l == r) {
            if (a[l - 1] == '0') {
                tree[o].state = -1;
            } else {
                tree[o].state = 1;
                tree[o].ones = 1;
            }
            return;
        }
        int m = (l + r) >> 1;
        buildWithBinary(a, o << 1, l, m);
        buildWithBinary(a, o << 1 | 1, m + 1, r);
        maintain(o);
    }

    // Flips bits in range [l, r]
    void flip(int o, int l, int r) {
        if (l <= tree[o].l && tree[o].r <= r) {
            doFlip(o);
            return;
        }
        spread(o);
        int m = (tree[o].l + tree[o].r) >> 1;
        if (l <= m) flip(o << 1, l, r);
        if (m < r) flip(o << 1 | 1, l, r);
        maintain(o);
    }

    // Returns the first index >= l with a 0, or -1 if not found
    int next0(int o, int l) {
        if (tree[o].l == tree[o].r) {
            return tree[o].state < 0 ? tree[o].l : -1;
        }
        spread(o);
        int m = (tree[o].l + tree[o].r) >> 1;
        if (l <= m && tree[o << 1].state <= 0) {
            int p = next0(o << 1, l);
            if (p > 0) return p;
        }
        return next0(o << 1 | 1, l);
    }

    // Returns the first index >= l with a 1, or -1 if not found
    int next1(int o, int l) {
        if (tree[o].l == tree[o].r) {
            return tree[o].state > 0 ? tree[o].l : -1;
        }
        spread(o);
        int m = (tree[o].l + tree[o].r) >> 1;
        if (l <= m && tree[o << 1].state >= 0) {
            int p = next1(o << 1, l);
            if (p > 0) return p;
        }
        return next1(o << 1 | 1, l);
    }

    // Returns the position of the k-th 1
    int kth1(int o, int k) {
        if (tree[o].l == tree[o].r) {
            return tree[o].l;
        }
        spread(o);
        if (k <= tree[o << 1].ones) return kth1(o << 1, k);
        return kth1(o << 1 | 1, k - tree[o << 1].ones);
    }

    // Returns the last index with a 1 (similar to bits.Len)
    int lastIndex1(int o) {
        if (tree[o].l == tree[o].r) {
            return tree[o].l;
        }
        spread(o);
        if (tree[o << 1 | 1].state >= 0) return lastIndex1(o << 1 | 1);
        return lastIndex1(o << 1);
    }

    // += 1<<i, simulating carry
    void add(int i) { flip(1, i, next0(1, i)); }

    // -= 1<<i, simulating borrow
    void sub(int i) { flip(1, i, next1(1, i)); }

    // Returns the count of 1s in the range [l, r]
    int onesCount(int o, int l, int r) {
        if (l <= tree[o].l && tree[o].r <= r) return tree[o].ones;
        spread(o);
        int m = (tree[o].l + tree[o].r) >> 1;
        if (r <= m) return onesCount(o << 1, l, r);
        if (m < l) return onesCount(o << 1 | 1, l, r);
        return onesCount(o << 1, l, r) + onesCount(o << 1 | 1, l, r);
    }

    // Equivalent functions
    bool getBit(int i) { return onesCount(1, i, i) == 1; }
    bool all0(int l, int r) { return onesCount(1, l, r) == 0; }
    bool all1(int l, int r) { return onesCount(1, l, r) == r - l + 1; }
    int index0() { return next0(1, 1); }
    int index1() { return next1(1, 1); }
    int trailingZeros() { return index1(); }
    int len() {
        return tree[1].state < 0 ? 0 : lastIndex1(1);
    }

    // To be implemented: setRange, resetRange
};
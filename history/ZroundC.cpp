#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class SegmentTree {
private:
    vector<long long> treeXor, treeOr, lazy; // treeXor 用于存储 XOR，treeOr 用于存储 OR
    int n;

    // Push down the lazy values
    void push(int node, int start, int end) {
        if (lazy[node] != 0) {
            // Apply XOR to current node
            treeXor[node] ^= lazy[node] * ((end - start + 1) % 2); // 如果区间长度为奇数，XOR 需要传播
            treeOr[node] |= lazy[node]; // OR 操作简单传播

            // If not a leaf node, propagate laziness to children
            if (start != end) {
                lazy[2 * node + 1] ^= lazy[node];
                lazy[2 * node + 2] ^= lazy[node];
            }
            lazy[node] = 0; // Clear the lazy value
        }
    }

    // Range XOR update
    void update(int node, int start, int end, int l, int r, int value) {
        push(node, start, end); // Ensure previous updates are applied

        if (start > r || end < l) {
            // No overlap
            return;
        }

        if (start >= l && end <= r) {
            // Total overlap
            lazy[node] ^= value; // Mark this node lazy
            push(node, start, end); // Push the current update immediately
            return;
        }

        // Partial overlap
        int mid = (start + end) / 2;
        update(2 * node + 1, start, mid, l, r, value);
        update(2 * node + 2, mid + 1, end, l, r, value);

        // After update, re-calculate the values for this node
        treeXor[node] = treeXor[2 * node + 1] ^ treeXor[2 * node + 2];
        treeOr[node] = treeOr[2 * node + 1] | treeOr[2 * node + 2];
    }

    // Range XOR query
    long long queryXor(int node, int start, int end, int l, int r) {
        push(node, start, end); // Ensure previous updates are applied

        if (start > r || end < l) {
            // No overlap
            return 0;
        }

        if (start >= l && end <= r) {
            // Total overlap
            return treeXor[node];
        }

        // Partial overlap
        int mid = (start + end) / 2;
        return queryXor(2 * node + 1, start, mid, l, r) ^
               queryXor(2 * node + 2, mid + 1, end, l, r);
    }

    // Range OR query
    long long queryOr(int node, int start, int end, int l, int r) {
        push(node, start, end); // Ensure previous updates are applied

        if (start > r || end < l) {
            // No overlap
            return 0;
        }

        if (start >= l && end <= r) {
            // Total overlap
            return treeOr[node];
        }

        // Partial overlap
        int mid = (start + end) / 2;
        return queryOr(2 * node + 1, start, mid, l, r) |
               queryOr(2 * node + 2, mid + 1, end, l, r);
    }

public:
    SegmentTree(int size) {
        n = size;
        treeXor.assign(4 * n, 0);
        treeOr.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
    }

    // Public update function
    void update(int l, int r, int value) {
        update(0, 0, n - 1, l, r, value);
    }

    // Public XOR query function
    long long queryXor(int l, int r) {
        return queryXor(0, 0, n - 1, l, r);
    }

    // Public OR query function
    long long queryOr(int l, int r) {
        return queryOr(0, 0, n - 1, l, r);
    }
};

int main() {
    int n, q;
    cin >> n >> q; // 输入数组大小和查询数
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i]; // 输入数组
    }

    SegmentTree segTree(n);

    // 初始化线段树
    for (int i = 0; i < n; ++i) {
        segTree.update(i, i, a[i
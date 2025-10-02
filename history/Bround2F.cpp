#include <bits/stdc++.h>
using namespace std;

using ll = long long;

/*
 * Node Structure:
 * Represents a single node in the Treap, storing a key-value pair,
 * the priority for balancing, count of keys, and cumulative sums
 * to allow efficient range and order-based operations.
 */
template<typename K>
struct Node {
    Node* son[2] = {nullptr, nullptr};  // Left and right children
    unsigned priority;  // Random priority for maintaining Treap balance
    K key;              // Key stored in the node
    K keySum;           // Sum of keys (key * keyCnt) at this node
    int keyCnt;         // Count of occurrences of this key
    int subSize;        // Total size of subtree rooted at this node
    K subSum;           // Sum of all keys in the subtree

    // Constructor for initializing a node with a given key, count, and priority
    Node(K k, int count, unsigned prio)
        : priority(prio), key(k), keyCnt(count), keySum(count * key), subSize(count), subSum(count * key) {}

    // Size of the subtree rooted at this node
    int size() const {
        return subSize;
    }

    // Sum of all keys in the subtree rooted at this node
    K getSum() const {
        return subSum;
    }

    // Update subtree size and sum based on the children
    void maintain() {
        subSize = keyCnt + (son[0] ? son[0]->size() : 0) + (son[1] ? son[1]->size() : 0);
        subSum = keySum + (son[0] ? son[0]->getSum() : 0) + (son[1] ? son[1]->getSum() : 0);
    }

    // Rotate the node to balance the tree, direction d is either 0 (left) or 1 (right)
    Node* rotate(int d) {
        Node* x = son[d ^ 1];
        son[d ^ 1] = x->son[d];
        x->son[d] = this;
        this->maintain();
        x->maintain();
        return x;
    }
};

/*
 * Treap Class:
 * A balanced binary search tree that maintains both BST and heap properties.
 * Each operation is logarithmic on average, and it supports order-based queries.
 */
template<typename K>
class Treap {
private:
    unsigned rd;  // Random seed for generating priorities
    Node<K>* root;  // Root node of the Treap
    std::function<int(const K&, const K&)> comparator;  // Comparator function for keys
    std::function<K(const K&)> keyToInt;  // Function to convert keys to integer values for sum calculations

    // Generate a pseudo-random number for priority
    unsigned fastRand() {
        rd ^= rd << 13;
        rd ^= rd >> 17;
        rd ^= rd << 5;
        return rd;
    }

    // Helper function to insert a key with a specified count
    Node<K>* put(Node<K>* o, K key, int num) {
        if (!o) {
            if (num < 0) throw std::invalid_argument("Negative count not allowed");
            return new Node<K>(key, num, fastRand());
        }
        int c = comparator(key, o->key);
        if (c == 0) {
            o->keyCnt += num;
            if (o->keyCnt < 0) throw std::invalid_argument("Negative key count");
            o->keySum += keyToInt(key) * num;
        } else {
            int d = (c > 0);
            o->son[d] = put(o->son[d], key, num);
            if (o->son[d]->priority > o->priority) {
                o = o->rotate(d ^ 1);
            }
        }
        o->maintain();
        return o;
    }

public:
    Treap()
        : rd(static_cast<unsigned>(time(nullptr)) / 2 + 1),
          root(nullptr),
          comparator([](const K& a, const K& b) { return a < b ? -1 : (a > b ? 1 : 0); }),
          keyToInt([](const K& key) { return key; }) {}

    // Insert `key` with `num` occurrences (positive to add, negative to remove)
    // num=1 表示添加一个 key
    // num=-1 表示移除一个 key
    void put(K key, int num) { root = put(root, key, num); }

    // Returns the index of the first element >= key, equivalent to "lower_bound"
    // 把 treap 当作一个有序数组，返回第一个 >= key 的数的下标，若不存在，返回 size()
    // 等价于 < key 的元素个数
    int lowerBoundIndex(K key) const {
        int kth = 0;
        Node<K>* o = root;
        while (o) {
            int c = comparator(key, o->key);
            if (c < 0) {
                o = o->son[0];
            } else {
                kth += (o->son[0] ? o->son[0]->size() : 0) + o->keyCnt;
                if (c == 0) break;
                o = o->son[1];
            }
        }
        return kth;
    }

    // Returns the index of the first element > key, equivalent to "upper_bound"
    // 把 treap 当作一个有序数组，返回第一个 > key 的数的下标，若不存在，返回 size()
    // 等价于 <= key 的元素个数
    int upperBoundIndex(K key) const {
        int kth = 0;
        Node<K>* o = root;
        while (o) {
            int c = comparator(key, o->key);
            if (c < 0) {
                o = o->son[0];
            } else {
                kth += (o->son[0] ? o->son[0]->size() : 0) + o->keyCnt;
                if (c == 0) {
                    kth += o->keyCnt;
                    break;
                }
                o = o->son[1];
            }
        }
        return kth;
    }

    // Returns the prefix sum of the first `k` elements in the Treap
    // 把 treap 当作一个有序数组，返回左闭右开区间 [0,k) 的元素和
    // 特别地，k = size() 表示整棵树的元素和
    K preSum(int k) const {
        K sum = 0;
        if (k < 0 || k > size()) throw std::out_of_range("Invalid range");
        Node<K>* o = root;
        while (o) {
            int leftSize = o->son[0] ? o->son[0]->size() : 0;
            if (k < leftSize) {
                o = o->son[0];
            } else {
                sum += o->son[0] ? o->son[0]->getSum() : 0;
                k -= leftSize;
                if (k <= o->keyCnt) {
                    sum += keyToInt(o->key) * k;
                    break;
                }
                sum += o->keySum;
                k -= o->keyCnt;
                o = o->son[1];
            }
        }
        return sum;
    }

    // Returns the sum of elements < x
    K sumLess(K x) const { return preSum(lowerBoundIndex(x)); }

    // Returns the sum of elements <= x
    K sumLessEqual(K x) const { return preSum(upperBoundIndex(x)); }

    // Returns the sum of elements > x
    K sumGreater(K x) const { return root ? root->getSum() - preSum(upperBoundIndex(x)) : 0; }

    // Returns the sum of elements >= x
    K sumGreaterEqual(K x) const { return root ? root->getSum() - preSum(lowerBoundIndex(x)) : 0; }

    // Returns the k-th smallest node in the Treap (0-based index)
    Node<K>* kth(int k) const {
        if (k < 0 || k >= size()) return nullptr;
        Node<K>* o = root;
        while (o) {
            int leftSize = o->son[0] ? o->son[0]->size() : 0;
            if (k < leftSize) {
                o = o->son[0];
            } else {
                k -= leftSize + o->keyCnt;
                if (k < 0) break;
                o = o->son[1];
            }
        }
        return o;
    }

    // Returns the node with the largest key < `key`
    // 求 v 的前驱（小于 key 的最大元素）
    Node<K>* prev(K key) const {
        return kth(lowerBoundIndex(key) - 1);
    }

    // Returns the node with the smallest key > `key`
    // 求 v 的后继（大于 key 的最小元素）
    Node<K>* next(K key) const {
        return kth(upperBoundIndex(key));
    }

    // <= key 可以用 t.kth(t.upperBoundIndex(key)-1)
    // >= key 可以用 t.kth(t.lowerBoundIndex(key))

    // Finds a node with a specific key
    Node<K>* find(K key) const {
        Node<K>* o = kth(lowerBoundIndex(key));
        if (o && o->key == key) {
            return o;
        }
        return nullptr;
    }

    // Returns the total number of elements in the Treap
    int size() const { return root ? root->size() : 0; }

    // Checks if the Treap is empty
    bool empty() const { return size() == 0; }
};

void solve() {
    int n, q;
    cin >> n >> q;

    vector<ll> co(n);
    vector<ll> a(n);
    vector<Treap<ll>> tr(n);

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    for (int i = 0; i < n; ++i) {
        cin >> co[i];
        --co[i];
    }

    vector<ll> cnt(n, 0);
    for (int i = 0; i < n; ++i) {
        cnt[co[i]]++;
        tr[co[i]].put(a[i], 1);
    }

    vector<ll> lazy(n, 0);

    while (q--) {
        int op;
        cin >> op;

        if (op == 1) {
            ll i, c;
            cin >> i >> c;
            --i;
            --c;

            tr[co[i]].put(a[i], -1);
            a[i] += lazy[co[i]];
            tr[c].put(a[i] - lazy[c], 1);
            a[i] -= lazy[c];

            cnt[co[i]]--;
            co[i] = c;
            cnt[c]++;
        } else if (op == 2) {
            ll c, v;
            cin >> c >> v;
            --c;
            lazy[c] += v;
        } else if (op == 3) {
            ll c, k;
            cin >> c >> k;
            --c;

            ll left = -1, right = cnt[c] + 1;
            while (left + 1 < right) {
                ll mid = (left + right) / 2;
                (tr[c].preSum(mid) + mid * lazy[c] <= k ? left : right) = mid;
            }
            cout << left << '\n';
        }
    }
}

#define MULTICASE false
int main() {
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}
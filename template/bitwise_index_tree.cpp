#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class BITree {
private:
    vector<int> nums;
    vector<int> tree;

    int lowbit(int u) { return u & -u; }

    int prefixSum(int i) {
        int s = 0;
        for (; i > 0; i -= lowbit(i)) {
            s += tree[i];
        }
        return s;
    }

public:
    BITree(vector<int>& nums) : nums(nums), tree(nums.size() + 1)  {
        for (int i = 1; i <= nums.size(); i++) {
            tree[i] += nums[i - 1];
            int nxt = i + lowbit(i);
            if (nxt <= nums.size()) {
                tree[nxt] += tree[i];
            }
        }
    }
    void update(int index, int val) {
        int delta = val - nums[index];
        nums[index] = val;
        for (int i = index + 1; i < tree.size(); i += lowbit(i)) {
            tree[i] += delta;
        }
    }

    int sumRange(int left, int right) {
        return prefixSum(right + 1) - prefixSum(left);
    }
};

void abbrForm() {
    auto lb = [](int u) { return u & -u; };
    auto add = [&](vector<ll> &tr, int pos, int val) { for (; pos < tr.size(); pos += lb(pos)) tr[pos] += val; };
    auto query = [&](vector<ll> &tr, int pos) { ll res = 0; for (; pos; pos -= lb(pos)) res += tr[pos]; return res; };
    vector<ll> tr;
    add(tr, 0, 1);
    query(tr, 0);
}

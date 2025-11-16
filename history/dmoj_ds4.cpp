#include <bits/stdc++.h>

using namespace std;

struct Node {
    Node *ch[2];
    int val;
    int rank;
    int rep_cnt;
    int siz;

    Node(int _val) : val(_val), rep_cnt(1), siz(1) {
        ch[0] = ch[1] = nullptr;
        rank = rand();
    }

    void upd_siz() {
        siz = rep_cnt;
        if (ch[0] != nullptr) siz += ch[0]->siz;
        if (ch[1] != nullptr) siz += ch[1]->siz;
    }
};


class Treap {
private:
    Node *root;
    enum rot_type { LF = 1, RT = 0 };

    void _rotate(Node *&cur, rot_type dir) {
        assert(cur != nullptr);
        Node *tmp = cur->ch[dir];
        cur->ch[dir] = tmp->ch[!dir];
        tmp->ch[!dir] = cur;
        cur->upd_siz(), tmp->upd_siz();
        cur = tmp;
    }

    void _insert(Node *&cur, int val) {
        if (cur == nullptr) {
            cur = new Node(val);
        } else if (val == cur->val) {
            cur->rep_cnt++;
            cur->siz++;
        } else if (val < cur->val) {
            _insert(cur->ch[0], val);
            if (cur->ch[0]->rank < cur->rank) {
                _rotate(cur, RT);
            }
            cur->upd_siz();
        } else {
            _insert(cur->ch[1], val);
            if (cur->ch[1]->rank < cur->rank) {
                _rotate(cur, LF);
            }
            cur->upd_siz();
        }
    }

    void _del(Node *&cur, int val) {
        if (cur == nullptr) return;
        if (val > cur->val) {
            _del(cur->ch[1], val);
            cur->upd_siz();
        } else if (val < cur->val) {
            _del(cur->ch[0], val);
            cur->upd_siz();
        } else {
            if (cur->rep_cnt > 1) {
                cur->rep_cnt--, cur->siz--;
                return;
            }
            uint8_t state = 0;
            state |= (cur->ch[0] != nullptr);
            state |= (cur->ch[1] != nullptr) << 1;
            Node *tmp = cur;
            switch (state) {
                case 0:
                    delete cur;
                    cur = nullptr;
                    break;
                case 1:
                    cur = tmp->ch[0];
                    delete tmp;
                    break;
                case 2:
                    cur = tmp->ch[1];
                    delete tmp;
                    break;
                case 3:
                    rot_type dir = cur->ch[0]->rank < cur->ch[1]->rank ? RT : LF;
                    _rotate(cur, dir);
                    _del(cur->ch[!dir], val);
                    cur->upd_siz();
                    break;
            }
        }
    }

    int _query_rank(Node *cur, int val) {
        int less_siz = cur->ch[0] == nullptr ? 0 : cur->ch[0]->siz;
        if (val == cur->val) {
            return less_siz + 1;
        } else if (val < cur->val) {
            if (cur->ch[0] != nullptr)
                return _query_rank(cur->ch[0], val);
            else
                return INT_MIN;
        } else {
            if (cur->ch[1] != nullptr)
                return less_siz + cur->rep_cnt + _query_rank(cur->ch[1], val);
            else
                return INT_MIN;
        }
    }

    int _query_val(Node *cur, int rank) {
        int less_siz = cur->ch[0] == nullptr ? 0 : cur->ch[0]->siz;
        if (rank <= less_siz) {
            return _query_val(cur->ch[0], rank);
        } else if (rank <= less_siz + cur->rep_cnt) {
            return cur->val;
        } else {
            return _query_val(cur->ch[1], rank - less_siz - cur->rep_cnt);
        }
    }

    void _print(Node *cur) {
        if (cur == nullptr) return;
        _print(cur->ch[0]);
        for (int i = 0; i < cur->rep_cnt; i++) cout << cur->val << ' ';
        _print(cur->ch[1]);
    }

public:
    void insert(int val) { _insert(root, val); }
    void del(int val) { _del(root, val); }
    int query_rank(int val) {
        if (root == nullptr) return -1;
        return _query_rank(root, val);
    }
    int query_val(int rank) {
        assert(rank >= 1 && rank <= size());
        assert(root != nullptr);
        return _query_val(root, rank);
    }
    void print() {
        if (root != nullptr) {
            _print(root);
            cout << '\n';
        }
    }
    int size() { return root == nullptr ? 0 : root->siz; }
    Treap() : root(nullptr) {}
};

void solve() {
    int n, m;
    cin >> n >> m;

    Treap t;

    t.print();
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        t.insert(x);
    }

    int ans = 0;
    while (m--) {
        char op;
        int x;
        cin >> op >> x;
        int v = x ^ ans;
        if (op == 'I') {
            t.insert(v);
        } else if (op == 'R') {
            t.del(v);
        } else if (op == 'S') {
            ans = t.query_val(v);
            cout << ans << '\n';
        } else if (op == 'L') {
            ans = t.query_rank(v);
            if (ans < 0) ans = -1;
            cout << ans << '\n';
        } else {
            assert(false);
        }
    }
    t.print();
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    solve();
}

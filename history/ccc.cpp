#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

#define DEBUG false

template<typename T>
void print(vector<T> &a, function<T(T x)> f = [](T x) { return x; }) {
    for(int i=0; i<a.size(); i++) {
        cout << f(a[i]) << " ";
    }
    cout << endl;
}

const int LETTER = 2;
const char START = '0';

struct Node {
    Node* son[LETTER]{};
    Node* fail;
    int len;
    int idx;
    Node(int len, int idx) : len(len), idx(idx) {}
};

struct Fraction {
    int p, q;
    Fraction() : p(0), q(1) {}
    Fraction(int n) : p(n), q(1) {}
    Fraction(int p, int q) : p(p), q(q) {}
    friend Fraction operator+(const Fraction& f, const Fraction& g) {
        int p = f.p * g.q + f.q * g.p;
        int q = f.q * g.q;
        int d = gcd(p, q);
        return Fraction(p / d, q / d);
    }
    friend Fraction operator*(const Fraction& f, const Fraction& g) {
        int p = f.p * g.p;
        int q = f.q * g.q;
        int d = gcd(p, q);
        return Fraction(p / d, q / d);
    }
    Fraction& operator+=(const Fraction& rhs){
        *this = *this + rhs;
        return *this;
    }
    friend ostream& operator<<(ostream& os, const Fraction& f) {
        if(f.p == 0) os << "0  ";
        else if(f.q == 1) os << f.p << "  ";
        else os << f.p << "/" << f.q;
        return os;
    }
};

struct AhoCorasick {
    Node* root = new Node(0, 0);
    int size = 1;
    set<int> ends;
    vector<int> idxs;
    vector<vector<Fraction>> M;

    void put(string& s) {
        auto cur = root;
        for (char b : s) {
            b -= START;
            if (cur->son[b] == nullptr) {
                cur->son[b] = new Node(cur->len + 1, size++);
            }
            cur = cur->son[b];
        }
        ends.insert(cur->idx);
    }

    void build_fail() {
        idxs = vector<int>(size);
        iota(idxs.begin(), idxs.end(), 0);
        for(int x : ends) cout << x << " "; cout << endl;
        function<int(int)> f = [&](int i) -> int { return ends.contains(i); };
        ranges::sort(idxs, {}, f);
        // print(idxs);
        {
            vector<int> inv(size);
            for(int i=0; i<size; i++) {
                inv[idxs[i]] = i;
            }
            idxs = move(inv);
        }
        // print(idxs);
        // Mij is the probability transfer from ith to jth
        Fraction PROB(1, LETTER);
        M = vector<vector<Fraction>>(size, vector<Fraction>(size));
        root->fail = root;
        queue<Node*> q;
        for (auto& son : root->son) {
            if (son == nullptr) {
                M[idxs[root->idx]][idxs[root->idx]] += PROB;
                son = root;
            } else {
                M[idxs[root->idx]][idxs[son->idx]] += PROB;
                son->fail = root;
                q.push(son);
            }
        }
        while (!q.empty()) {
            // for(int i=0; i<M.size(); i++) print(M[i]); cout << endl;
            auto cur = q.front();
            q.pop();
            bool end = ends.contains(cur->idx);
            if(end) M[idxs[cur->idx]][idxs[cur->idx]] += Fraction(1);
            for (int i = 0; i < LETTER; i++) {
                auto& son = cur->son[i];
                if (son == nullptr) {
                    son = cur->fail->son[i];
                    if(!end) M[idxs[cur->idx]][idxs[son->idx]] += PROB;
                    continue;
                }
                if(!end) M[idxs[cur->idx]][idxs[son->idx]] += PROB;
                son->fail = cur->fail->son[i];
                q.push(son);
            }
        }
    }
};


void solve() {
    int n; cin >> n;
    AhoCorasick ac;
    for(int i=0; i<n; i++) {
        string s; cin >> s;
        ac.put(s);
    }
    ac.build_fail();
    vector<vector<Fraction>> M = ac.M;
    for(int i=0; i<M.size(); i++) print(M[i]);
    // for(int i=0; i<M.size(); i++) M[i][i] += Fraction(-1);
    // for(int i=0; i<M.size(); i++) print(M[i]);
}

#define MULTICASE true

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/in.in)", "r", stdin);
    // freopen(R"(/Users/alanlee/Desktop/CodeForce/TEST/out.out)", "w", stdout);
    // init();
    int t = 1;
#if MULTICASE
    cin >> t;
#endif
    while (t--) {
        solve();
    }
    return 0;
}

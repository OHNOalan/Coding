#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, w;

    Edge() {
    }
    Edge(int u, int v, int w) : u(u), v(v), w(w) {
    }
    bool operator<(const Edge& e) const {
        return w < e.w;
    }
};
#include <bits/stdc++.h>

using namespace std;

const int MX = 220;// (R + 1) * C + R * (C + 1)

// row k: (2 * M + 1) * r is base : r in [0, N]
// (2 * M + 1) * r + c: c in [0, M)
// col k: c in [0, M]
// (M + c) + (2 * M + 1) * r: r in [0, N)
// house r, c:
// top: (2 * M + 1) * r + c
// bot: (2 * M + 1) * (r + 1) + c
// lf:  (2 * M + 1) * r + M + c
// rt:  (2 * M + 1) * r + M + c + 1


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int R, C; cin >> R >> C;
    vector<bitset<MX>> st;
    for(int r=0; r<=R; r++) {
        bitset<MX> b;
        for(int c=0; c<C; c++) {
            b.set((2 * C + 1) * r + c);
        }
        st.push_back(b);
    }
    for(int c=0; c<=C; c++) {
        bitset<MX> b;
        for(int r=0; r<R; r++) {
            b.set((2 * C + 1) * r + (C + c));
        }
        st.push_back(b);
    }
    for(int r=0; r<R; r++) {
        for(int c=0; c<C; c++) {
            bitset<MX> b;
            b.set(((2 * C + 1) * r + c));
            b.set(((2 * C + 1) * (r + 1) + c));
            b.set(((2 * C + 1) * r + C + c));
            b.set(((2 * C + 1) * r + C + c + 1));
            st.push_back(b);
        }
    }
    vector<bitset<MX>> bs(MX);
    for(auto b : st) {
        for(int i=MX-1; i>=0; i--) {
            if(!b[i]) continue;
            if(bs[i] == 0) {
                bs[i] = b;
                break;
            }
            b ^= bs[i];
        }
    }

    bitset<MX> B;
    for(int i = 0; i < (R + 1) * C + R * (C + 1); i++) {
        int x; cin >> x;
        if(x) B.flip(i);
    }
    for(int i = 0; i < (R + 1) * C + R * (C + 1); i++) {
        int x; cin >> x;
        if(x) B.flip(i);
    }
    for(int i=MX-1; i>=0; i--) {
        if(B[i]) {
            if(bs[i] == 0) {
                break;
            }
            B ^= bs[i];
        }
    }

    cout << (B == 0 ? "Yes" : "No") << '\n';
    return 0;
} 

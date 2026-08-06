#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;
using u128 = unsigned __int128;

void solve() {
    int n;
    std::cin >> n;

    std::vector<int> l(n), r(n);
    for (int i = 0; i < n; i++) {
        std::cin >> l[i] >> r[i];
    }

    std::vector<int> L(n, -1), R(n, -1);

    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 0);

    {
        std::sort(p.begin(), p.end(),
            [&](int i, int j) {
                if (l[i] != l[j]) {
                    return l[i] < l[j];
                }
                return r[i] > r[j];
            });
        std::set<int> s;
        for (int j = 0; j < n; j++) {
            int i = p[j];
            auto it = s.lower_bound(r[i]);
            if (it != s.end()) {
                R[i] = *it;
            }
            s.insert(r[i]);
            if (j + 1 < n && l[i] == l[p[j + 1]] && r[i] == r[p[j + 1]]) {
                R[i] = r[i];
            }
        }
    }

    {
        std::sort(p.begin(), p.end(),
            [&](int i, int j) {
                if (r[i] != r[j]) {
                    return r[i] > r[j];
                }
                return l[i] < l[j];
            });
        std::set<int> s;
        for (int j = 0; j < n; j++) {
            int i = p[j];
            auto it = s.upper_bound(l[i]);
            if (it != s.begin()) {
                L[i] = *std::prev(it);
            }
            s.insert(l[i]);
            if (j + 1 < n && l[i] == l[p[j + 1]] && r[i] == r[p[j + 1]]) {
                L[i] = l[i];
            }
        }
    }

    for (int i = 0; i < n; i++) {
        int ans;
        if (L[i] == -1) {
            ans = 0;
        } else {
            ans = R[i] - L[i] - (r[i] - l[i]);
        }
        std::cout << ans << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;

    while (t--) {
        solve();
    }

    return 0;
}



//#include <bits/stdc++.h>
//using namespace std;
//
//using ll = long long;
//using ull = unsigned long long;
//
//#define DEBUG false
//
//void solve() {
//    int n; cin >> n;
//    vector<array<int, 2>> a(n);
//    for (int i = 0; i < n; i++) {
//        cin >> a[i][0] >> a[i][1];
//    }
//    set<int> st;
//    vector<int> left(n, -1), right(n, -1);
//    ranges::sort(a);
//    for(int i=0; i<n; i++) {
//        while(st.size() && *st.begin() < a[i][0]) {
//            st.erase(st.begin());
//        }
//        auto it = lower_bound(st.begin(), st.end(), a[i][1]);
//        if(it != st.end()) right[i] = *it;
//        st.insert(a[i][1]);
//    }
//    ranges::sort(a, [&](auto a, auto b) {
//        return a[1] < b[1] || (a[1] == b[1] && a[0] < b[0]);
//    });
//    for(int i=n-1; i>=0; i--) {
//        while(st.size() && *st.begin() > a[i][1]) {
//            st.erase(st.begin());
//        }
//        auto it = lower_bound(st.begin(), st.end(), a[i][0] + 1);
//        if(it != st.end() && it != st.begin()) left[i] = *prev(it);
//        st.insert(a[i][0]);
//    }
//    vector<int> ans(n);
//    for(int i=0; i<n; i++) {
//        if(left[i] != -1 && right[i] != -1) {
//            ans[i] = right[i] - left[i] + 1 - (a[i][1] - a[i][0] + 1);
//        }
//    }
//    for(int i=0; i<n; i++) {
//        cout << ans[i] << '\n';
//    }
//}
//
//#define MULTICASE true
//
//int main() {
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);
//    cout.tie(nullptr);
//    // init();
//    int t = 1;
//#if MULTICASE
//    cin >> t;
//#endif
//    while (t--) {
//        solve();
//    }
//    return 0;
//}

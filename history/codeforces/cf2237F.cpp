#include <bits/stdc++.h>

using namespace std;

void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    vector<int> dp(n), dpst(n), dped(n), pre(n + 1), suf(n + 1);

    for (int i = 0; i < n; i++) {
        int st = i - a[i] + 1, ed = st + m - 1;
        bool valid = (st >= 0 && ed < n);
        if (valid) {
            dp[i] = max(pre[st], max(dpst[st], suf[i])) + 1;
            dpst[st] = max(dpst[st], dp[i]);
            dped[ed] = max(dped[ed], dp[i]);
        }
        pre[i + 1] = max(pre[i], dp[i]);
        suf[i + 1] = max(suf[i], dped[i]);
    }
    cout << n - suf[n] << '\n';
}

void solve2() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    vector<int> dpst(n, 0); // 起点为 s 的段：活跃窗口 [s, s+m-1]，不能合并
    vector<int> pre(n + 1, 0);
    vector<int> suf(n + 1, 0); // suf 兼职 dped：ed+1 处累积，随后作为滚动前缀最大值读出

    for (int i = 0; i < n; i++) {
        int st = i - a[i] + 1, ed = st + m - 1;
        bool valid = (st >= 0 && ed < n);
        int cur = 0;
        if (valid) {
            cur = max(pre[st], max(dpst[st], suf[i])) + 1;
            dpst[st] = max(dpst[st], cur);
            suf[ed + 1] = max(suf[ed + 1], cur);
        }
        pre[i + 1] = max(pre[i], cur);
        suf[i + 1] = max(suf[i + 1], suf[i]);
    }
    cout << n - suf[n] << '\n';
}

void solve3() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    vector<int> dpst(n, 0);
    int pre = 0, bound = 0;

    for (int i = 0; i < n; i++) {
        int st = i - a[i] + 1, ed = st + m - 1;
        bool valid = (st >= 0 && ed < n);

        dpst[i] = pre;
        int cur = 0;
        if (valid) {
            cur = max(dpst[st], bound) + 1;
            dpst[st] = max(dpst[st], cur);
            pre = max(pre, cur);
        }
        int closeSt = i - m + 1;
        if (closeSt >= 0) bound = max(bound, dpst[closeSt]);
    }
    cout << n - bound << '\n';
}


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    cin >> t;
    while (t--) solve();
}

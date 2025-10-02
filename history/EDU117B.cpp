#include <bits/stdc++.h>
using namespace std;
void solve() {
    int n, a, b;
    cin >> n >> a >> b;

    if(a >= b) {
        if(a == n/2 + 1 && b == n/2) {
            for(int i=n; i>0; i--) {
                cout << i << " ";
            }
            cout << endl;
        } else {
            cout << -1 << endl;
        }
        return;
    }

    if (a > n / 2 || b <= n / 2) {
        cout << -1 << '\n';
        return;
    }


    vector<int> left = {a};
    vector<int> right = {b};

    for(int i=n; i>b; i--)
        left.push_back(i);
    for(int i=1; i<a; i++)
        right.push_back(i);

    for(int i=1; i<=b-n/2-1; i++)
        left.push_back(a+i);

    for(int i=1; i<=n/2-a; i++)
        right.push_back(b-i);

    for (auto num : left)
        cout << num << ' ';
    for (auto num : right)
        cout << num << ' ';
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    cin >> t;
    while (t--) solve();
}

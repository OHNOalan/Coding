#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    sort(a.begin(), a.end());
    if(a[0] == 0) {
        cout << "NO\n";
        return;
    }
    cout << "YES\n";
    for(int i=0, j=n-1, sum=0; i<=j;) {
        if(sum >= 0) {
            cout << a[i] << " ";
            sum += a[i++];
        } else {
            cout << a[j] << " ";
            sum += a[j--];
        }
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) solve();
}

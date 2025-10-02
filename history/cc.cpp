#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;

    vector<int> A(N);
    vector<int> B(N);

    for (int i = 0; i < N; ++i)
        cin >> A[i];
    for (int i = 0; i < N; ++i)
        cin >> B[i];


    int max_len = 0;
    int l0 = 0, sumA0 = 0, xorB0 = 0;
    int l1 = 0, sumA1 = 0, xorB1 = 0;
    int l2 = 0, sumA2 = 0, xorB2 = 0;

    for (int r = 0; r < N; r++) {
        sumA2 += A[r];
        xorB2 ^= B[r];
        while (l2 <= r && sumA2 > 2) {
            sumA2 -= A[l2];
            xorB2 ^= B[l2];
            l2++;
        }
        if (sumA2 - (sumA2 > 0) == xorB2) {
            max_len = max(max_len, r - l2 + 1);
        }

        sumA1 += A[r];
        xorB1 ^= B[r];
        while (l1 <= r && sumA1 > 1) {
            sumA1 -= A[l1];
            xorB1 ^= B[l1];
            l1++;
        }
        if (sumA1 - (sumA1 > 0) == xorB1) {
            max_len = max(max_len, r - l1 + 1);
        }

        sumA0 += A[r];
        xorB0 ^= B[r];
        while (l0 <= r && sumA0 > 0) {
            sumA0 -= A[l0];
            xorB0 ^= B[l0];
            l0++;
        }
        if (sumA0 - (sumA0 > 0) == xorB0) {
            max_len = max(max_len, r - l0 + 1);
        }
    }

    cout << max_len << '\n';

    return 0;
}

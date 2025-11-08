#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

// no validation needed

// 2 * (n + n + 1 + 1)
// 1 + 1 + 1 + 1;

// [l, r)
// 0 <= x, y < n
void game() {
    int n;
    cin >> n;
    // 0: nothing, 1: play A, 2 play B
    vector<vector<int>> a(n, vector<int>(n));

    // no overflow, input checking
    int available_spot = n * n;

    auto tie = [&]() -> bool { return available_spot <= 0; };

    auto win = [&](int x, int y, int U) -> bool {
        // row check;
        bool row = true;
        for (int i = 0; i < n; i++) {
            if (a[x][i] != U) {
                row = false;
                break;
            }
        }
        if (row) return true;

        // column
        bool column = true;
        for (int i = 0; i < n; i++) {
            if (a[i][y] != U) {
                column = false;
                break;
            }
        }
        if (column) return true;

        // diagonal
        if (x == y) {
            bool diagonal = true;
            for (int i = 0; i < n; i++) {
                if (a[i][i] != U) {
                    diagonal = false;
                    break;
                }
            }
            if (diagonal) return true;
        }

        // anti-diagonal
        if (n - 1 - y == x) {
            bool anti_diagonal = true;
            for (int i = 0; i < n; i++) {
                if (a[i][n - 1 - i] != U) {
                    anti_diagonal = false;
                    break;
                }
            }
            if (anti_diagonal) return true;
        }
        return false;
    };

    // 1, 2
    int curPlayer = 1;
    while (!tie()) {
        int x, y;
        while (true) {
            cin >> x >> y;
            // validate, check range and if grid is placed before
            break;
        }
        a[x][y] = curPlayer;
        available_spot -= 1;
        if (win(x, y, curPlayer)) {
            cout << "win" << endl;
            return;
        }
        curPlayer = curPlayer == 1 ? 2 : 1;
    }
    cout << "tie: stope game" << endl;
}


// 1. ask clarification whenever you need
// 2. Don't make assumption


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    game();
}

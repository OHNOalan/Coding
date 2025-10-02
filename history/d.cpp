class Solution {
public:
    int countWinningSequences(string s) {
        const int mod = 1e9 + 7;
        int n = s.size();
        vector<vector<vector<int>>> f(2, vector<vector<int>>(3, vector<int>(2 * n + 1, 0)));
        int sc[3][3] = {{0, 1, -1}, {-1, 0, 1}, {1, -1, 0}}; // [alice][bob]
        map<char, int> ma{{'F',0}, {'W', 1}, {'E', 2}};
        for(int j=0; j<3; j++) {
            int c = ma[s[0]];
            int score = sc[c][j];
            f[0][j][n+score] = 1;
        }
        for(int i=1; i<n; i++) {
            for(int j=0; j<3; j++) {
                int c = ma[s[i]];
                int score = sc[c][j];
                for(int k=0; k<=2*n; k++) {
                    for(int j1 = 0; j1<3; j1++) {
                        if(j1 != j && k-score >= 0 && k-score <= 2*n) 
                            f[i%2][j][k] += f[(i-1)%2][j1][k-score], f[i%2][j][k] %= mod;
                    }
                }
            }
        }
        int ans = 0;
        for(int j=0; j<3; j++) {
            for(int k=n+1; k<=2*n; k++) {
                ans += f[(n-1)%2][j][k];
                ans %= mod;
            }
        }
        return ans;
    }
};
#include <bits/stdc++.h>

const int mod = 1e9 + 7;
long long quickMul(long long a, long long b) {
    long long res = 1;
    while(b) {
        if(b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

int calculateWays(int wordLen, int maxVowels) {
    if(maxVowels == 0) return quickMul(21, wordLen);
    vector<long long> prev(maxVowels + 1);
    prev[0] = 21;
    if(maxVowels >= 1) prev[1] = 5;
    for(int i=2; i<=wordLen; i++) {
        vector<long long> curr(maxVowels + 1);
        long long prefSum = 0;
        
        for(long long num : prev) {
            prefSum = (prefSum + num) % mod;
        }
        
        curr[0] = (prefSum * 21) % mod;
        
        for(int k=1; k<=maxVowels; k++) {
            if(k-1 < prev.size()) {
                curr[k] = (prev[k-1] * 5) % mod;
            }
        }
        prev = curr;
    }
    
    long long total = 0;
    for(long long num : prev) {
        total = (total + num) % mod;
    }
    return total;
}
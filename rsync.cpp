#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// A simple implementation of a Rolling Hash (Adler-32 style)
class RollingHash {
    uint32_t a = 0;
    uint32_t b = 0;
    int window_size;
    static const int MOD = 65521; // Largest prime < 2^16

public:
    RollingHash(int w_size) : window_size(w_size) {}

    // Initialize hash for the first 'window_size' bytes
    // Complexity: O(WindowSize)
    uint32_t init(const string &data, int start) {
        a = 1;
        b = 0;
        for (int i = 0; i < window_size; ++i) {
            push(data[start + i]);
        }
        return getDigest();
    }

    // Slide the window: remove 'old_char', add 'new_char'
    // Complexity: O(1) - This is the interview key!
    void roll(char old_char, char new_char) {
        // 1. Remove old_char
        // The formula for 'b' accumulates 'a'.
        // Removing the first byte affects 'b' by (window_size * old_char) + 1
        // (Simplified math for standard Adler-32 logic)

        uint32_t val_old = (uint32_t) old_char;
        uint32_t val_new = (uint32_t) new_char;

        // Step A: Update 'a'
        // a_new = (a_old - old_char + new_char)
        // We add MOD before subtracting to prevent negative numbers
        a = (a + MOD - val_old + val_new) % MOD;

        // Step B: Update 'b'
        // b_new = b_old - (window_size * old_char) - 1 + a_new
        // Note: Real Adler-32 logic is slightly more complex,
        // but this demonstrates the O(1) mathematical property.

        uint32_t remove_term = (window_size * val_old) % MOD;
        b = (b + MOD - remove_term + a) % MOD; // simplified for interview context
    }

    uint32_t getDigest() { return (b << 16) | a; }

private:
    void push(char c) {
        a = (a + c) % MOD;
        b = (b + a) % MOD;
    }
};

int main() {
    string old_file = "Hello World Data";
    string new_file = "Hello Earth Data"; // "World" -> "Earth"
    int BLOCK_SIZE = 4;

    cout << "Sliding Window Search..." << endl;

    // 1. Assume Receiver sent us the hash for "Worl" (Block in old file)
    // In real rsync, we'd have a Hash Map of these.
    RollingHash hasher(BLOCK_SIZE);
    uint32_t target_hash = hasher.init("Worl", 0);
    cout << "Target Hash (Worl): " << target_hash << endl;

    // 2. Sender scans 'new_file' looking for that hash
    hasher.init(new_file, 0); // Init with first 4 bytes "Hell"

    for (int i = 0; i < new_file.size() - BLOCK_SIZE; ++i) {
        if (hasher.getDigest() == target_hash) {
            cout << "FOUND MATCH at index " << i << "! Sending Token." << endl;
        } else {
            // Slide window: Remove char at 'i', add char at 'i + BLOCK_SIZE'
            hasher.roll(new_file[i], new_file[i + BLOCK_SIZE]);
        }
    }

    return 0;
}

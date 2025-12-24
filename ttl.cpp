#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

// The data stored in our Cache
struct CacheEntry {
    double price;

    // Timestamps (Unix millis)
    long long hard_expiry; // Absolute death
    long long soft_expiry; // When we should start refreshing
};

class SmartCache {
    // In a real system, this is Redis.
    // Here we simulate it with a simple class member for demo purposes.
    CacheEntry stored_entry;
    bool has_entry = false;
    mutex entry_lock;

public:
    // Helper to get current time
    long long now() {
        return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    }

    // Main Accessor
    double getPrice(string isbn) {
        // 1. Check Cache
        {
            lock_guard<mutex> lock(entry_lock);
            if (!has_entry) {
                // Cache Miss (Cold): Must fetch sync
                return fetchSynchronously(isbn);
            }

            long long current_time = now();

            // Case A: Hard Expired?
            if (current_time > stored_entry.hard_expiry) {
                // Must fetch sync (User waits)
                return fetchSynchronously(isbn);
            }

            // Case B: Soft Expired? (The Magic)
            if (current_time > stored_entry.soft_expiry) {
                // 1. Return the STALE data immediately (User is happy, 0 latency)
                double stale_price = stored_entry.price;

                // 2. Trigger Async Refresh
                // Use std::thread::detach or a ThreadPool to run in background
                thread([this, isbn]() { this->refreshBackground(isbn); }).detach();

                cout << "[Cache] Returning Stale ($" << stale_price << ") & Triggering Background Refresh..." << endl;
                return stale_price;
            }

            // Case C: Fresh
            return stored_entry.price;
        }
    }

private:
    // Simulates calling Amazon API
    double fetchFromAmazon() {
        this_thread::sleep_for(chrono::milliseconds(200)); // Latency
        return 25.00; // Price changed to $25
    }

    // Blocking fetch (updates cache)
    double fetchSynchronously(string isbn) {
        cout << "[Cache] Sync Fetch (User Waiting)..." << endl;
        double price = fetchFromAmazon();
        updateCache(price);
        return price;
    }

    // Background fetch (updates cache)
    void refreshBackground(string isbn) {
        // Note: In production, use a mutex/flag to ensure
        // only ONE background thread refreshes at a time (coalescing)
        cout << "[BgJob] Fetching new price..." << endl;
        double price = fetchFromAmazon();

        lock_guard<mutex> lock(entry_lock);
        updateCache(price);
        cout << "[BgJob] Cache Updated to $" << price << endl;
    }

    void updateCache(double price) {
        stored_entry.price = price;
        // Soft TTL: 5 seconds
        stored_entry.soft_expiry = now() + 5000;
        // Hard TTL: 10 seconds
        stored_entry.hard_expiry = now() + 10000;
        has_entry = true;
    }
};

int main() {
    SmartCache cache;

    // 1. First fetch (Cold)
    cout << "User 1: " << cache.getPrice("123") << endl;

    // 2. Wait 6 seconds (Passes Soft TTL of 5s, but inside Hard TTL of 10s)
    this_thread::sleep_for(chrono::seconds(6));

    // 3. User 2 asks
    // Should return OLD price immediately, but trigger update
    cout << "User 2: " << cache.getPrice("123") << endl;

    // Give background thread time to finish
    this_thread::sleep_for(chrono::seconds(1));

    // 4. User 3 asks (Should see NEW price)
    cout << "User 3: " << cache.getPrice("123") << endl;

    return 0;
}

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <semaphore> // C++20
#include <thread>
#include <vector>
using namespace std;

class ThreadPool {
private:
    queue<function<void()>> tasks;
    vector<thread> workers;

    mutex queue_mutex;
    condition_variable cv;

    bool stop;

public:
    ThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; i++) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(this->queue_mutex);
                        this->cv.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

                        if (this->stop && this->tasks.empty()) {
                            return;
                        }

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    void submit(function<void()> task) {
        {
            unique_lock<mutex> lock(queue_mutex);
            if (stop) {
                throw runtime_error("enqueue on stopped ThreadPool");
            }
            tasks.emplace(std::move(task));
        }
        cv.notify_one();
    }
    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stop = true;
        }
        cv.notify_all();
        for (thread &worker: workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
};

// Add this helper to prevent garbled cout output from multiple threads
mutex print_mutex;
void safe_print(string msg) {
    lock_guard<mutex> lock(print_mutex);
    cout << msg << endl;
}

void testThreadPool() {
    cout << "Creating ThreadPool with 4 threads..." << endl;
    ThreadPool pool(4);

    // Submit 8 tasks
    for (int i = 0; i < 8; ++i) {
        pool.submit([i] {
            // Simulate work
            safe_print("Task " + to_string(i) + " started by Thread " +
                       to_string(hash<thread::id>{}(this_thread::get_id()) % 1000));

            this_thread::sleep_for(chrono::seconds(1));

            safe_print("Task " + to_string(i) + " FINISHED.");
        });
    }

    cout << "Main thread submitted all tasks. Exiting main..." << endl;

    // ~ThreadPool() is called here automatically.
    // It will BLOCK until all 8 tasks are done.
}

template<typename T>
class BoundedBlockingQueue {
private:
    queue<T> q;
    int capacity;

    mutex mtx;
    condition_variable not_full; // "Hey Producer, there is space now"
    condition_variable not_empty; // "Hey Consumer, there is data now"

public:
    BoundedBlockingQueue(int cap) : capacity(cap) {}

    // Producer calls this
    void push(T item) {
        unique_lock<mutex> lock(mtx);

        // 1. Wait while full
        // We wait on 'not_full'. Only a Consumer can wake us up.
        while (q.size() >= capacity) {
            not_full.wait(lock);
        }

        // 2. Add item
        q.push(item);

        // 3. Notify ONE waiting consumer
        // We added data, so the queue is definitely not empty anymore.
        not_empty.notify_one();
    }

    // Consumer calls this
    T pop() {
        unique_lock<mutex> lock(mtx);

        // 1. Wait while empty
        // We wait on 'not_empty'. Only a Producer can wake us up.
        while (q.empty()) {
            not_empty.wait(lock);
        }

        // 2. Remove item
        T item = q.front();
        q.pop();

        // 3. Notify ONE waiting producer
        // We removed data, so there is definitely space now.
        not_full.notify_one();

        return item;
    }

    // Helper for testing
    int size() {
        lock_guard<mutex> lock(mtx);
        return q.size();
    }
};

void testBoundedBlockingQueue() {
    BoundedBlockingQueue<int> q(2); // Capacity 2

    thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            cout << "Pushing " << i << endl;
            q.push(i); // Will BLOCK here when i=2 until consumer catches up
        }
    });

    thread consumer([&]() {
        this_thread::sleep_for(chrono::seconds(1)); // Make producer fill up first
        for (int i = 0; i < 5; ++i) {
            int val = q.pop();
            cout << "Popped " << val << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    });

    producer.join();
    consumer.join();
}

template<typename T>
class SemaphoreQueue {
private:
    queue<T> q;
    mutex mtx; // Still need this to protect the std::queue push/pop

    // 1. Controls "Write Access" (Starts at Capacity)
    // "How many empty slots do we have?"
    std::counting_semaphore<> empty_slots;

    // 2. Controls "Read Access" (Starts at 0)
    // "How many items do we have?"
    std::counting_semaphore<> full_slots;

public:
    SemaphoreQueue(int capacity) : empty_slots(capacity), full_slots(0) {}

    void push(T item) {
        // 1. P(empty_slots) - Acquire a free slot
        // If count is 0, this BLOCKS automatically/efficiently
        empty_slots.acquire();

        // 2. Critical Section (Buffer access)
        {
            lock_guard<mutex> lock(mtx);
            q.push(item);
        }

        // 3. V(full_slots) - Signal that there is new data
        full_slots.release();
    }

    T pop() {
        // 1. P(full_slots) - Acquire an item
        // If count is 0 (no data), this BLOCKS
        full_slots.acquire();

        T item;
        // 2. Critical Section
        {
            lock_guard<mutex> lock(mtx);
            item = q.front();
            q.pop();
        }

        // 3. V(empty_slots) - Signal that we made space
        empty_slots.release();

        return item;
    }
};

void testSemaphoreQueue() {
    SemaphoreQueue<int> q(2);

    jthread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            cout << "Pushing " << i << endl;
            q.push(i); // Will BLOCK here when i=2 until consumer catches up
        }
    });

    jthread consumer([&]() {
        this_thread::sleep_for(chrono::seconds(1)); // Make producer fill up first
        for (int i = 0; i < 5; ++i) {
            int val = q.pop();
            cout << "Popped " << val << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    });
}

struct Task {
    long long execute_time; // Unix timestamp in ms
    function<void()> func;

    Task(long long t, function<void()> f) : execute_time(t), func(move(f)) {}

    // Operator for Min-Heap (We want the SMALLEST time at top)
    bool operator>(const Task &other) const { return execute_time > other.execute_time; }
};

class Scheduler {
    priority_queue<Task, vector<Task>, greater<Task>> pq;
    mutex mtx;
    condition_variable cv;
    bool stop = false;
    thread worker_thread;

public:
    Scheduler() {
        worker_thread = thread([this] { this->run(); });
    }
    ~Scheduler() {
        {
            lock_guard<mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        if (worker_thread.joinable()) worker_thread.join();
    }
    void schedule(function<void()> func, long long timestamp_ms) {
        lock_guard<mutex> lock(mtx);
        pq.emplace(timestamp_ms, func);

        // CRITICAL OPTIMIZATION:
        // If this new task is the EARLIEST one (it's at the top),
        // we must wake up the worker immediately.
        // Why? The worker might be sleeping for a task 1 hour away,
        // but this new task is due in 1 minute!
        if (pq.top().execute_time == timestamp_ms) {
            cv.notify_one();
        }
    }

private:
    void run() {
        unique_lock<mutex> lock(mtx);

        while (!stop || !pq.empty()) {
            // Case 1: Queue is empty
            if (pq.empty()) {
                if (stop) break;
                // Nothing to do, wait indefinitely for a new task
                cv.wait(lock);
            } else {
                long long now = getCurrentTimeMillis();
                long long due_time = pq.top().execute_time;

                if (now >= due_time) {
                    // Case 2: Task is due NOW (or in the past)
                    Task t = pq.top();
                    pq.pop();

                    // Unlock while running to allow concurrent schedule() calls
                    lock.unlock();
                    t.func();
                    lock.lock();
                } else {
                    // Case 3: Task is due in the FUTURE
                    // Wait UNTIL that specific time.
                    // This is better than sleep_for because it can be interrupted
                    // if 'notify_one' is called by a new, earlier task.

                    auto time_point = chrono::system_clock::time_point(chrono::milliseconds(due_time));

                    cv.wait_until(lock, time_point);

                    // When we wake up here, it's either:
                    // 1. Timeout reached (Task is due!)
                    // 2. Notified (A new, EARLIER task was added!)
                    // The loop restarts and re-checks pq.top() automatically.
                }
            }
        }
    }
    long long getCurrentTimeMillis() {
        return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    }
};

int main() {
    {
        vector<thread> threads;
        threads.emplace_back(testThreadPool);
        threads.emplace_back(testBoundedBlockingQueue);
        threads.emplace_back(testSemaphoreQueue);
    }
    cout << "done!" << endl;
}

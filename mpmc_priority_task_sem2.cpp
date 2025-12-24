#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <semaphore>
#include <sstream>
#include <thread>
#include <vector>


class thread_safe_print {
    std::stringstream buffer;
    inline static std::mutex output_mutex; // Static mutex shared by all instances

public:
    // Destructor: locks and prints everything at once
    ~thread_safe_print() {
        std::lock_guard<std::mutex> lock(output_mutex);
        std::cout << buffer.rdbuf();
    }

    // Helper to capture output
    template<typename T>
    thread_safe_print &operator<<(const T &msg) {
        buffer << msg;
        return *this;
    }

    // Support for manipulators like std::endl
    thread_safe_print &operator<<(std::ostream &(*func)(std::ostream &) ) {
        buffer << func;
        return *this;
    }
};

constexpr std::ptrdiff_t MAX_CAP = std::numeric_limits<std::ptrdiff_t>::max();

std::chrono::steady_clock::time_point get_start_time() {
    static const auto start = std::chrono::steady_clock::now();
    return start;
}

// A helper function to get elapsed seconds since start
double get_uptime_seconds() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - get_start_time();
    return elapsed.count();
}

struct Task {
    static constexpr int RATE = 100;
    int id;
    int priority;
    double arrive_time;
    double final_priority;
    std::function<void()> task;
    Task(int id, int priority, std::function<void()> task) :
        id(id), priority(priority), arrive_time(get_uptime_seconds()), final_priority(priority * RATE - arrive_time),
        task(task) {}
    bool operator<(const Task &other) const { return final_priority < other.final_priority; }
};


class MPMCPriority {
private:
    std::atomic<int> id_counter{0};
    // 1. Semaphores for Bounded Buffer
    std::counting_semaphore<MAX_CAP> slots_available; // Init to Capacity
    std::counting_semaphore<MAX_CAP> items_available; // Init to 0

    std::priority_queue<Task> tasks;
    std::mutex pq_mutex; // Protects ONLY the priority_queue
    bool logging;

public:
    MPMCPriority(int capacity, bool logging = false) :
        slots_available(capacity), items_available(0), logging(logging) {}

    void push(std::function<void()> task_func, int priority) {
        // 1. Wait for a free slot (Decrease Empty Count)
        slots_available.acquire();

        int t_id = id_counter.fetch_add(1, std::memory_order_relaxed);

        {
            std::lock_guard<std::mutex> lock(pq_mutex);
            // Construct in place
            tasks.emplace(t_id, priority, std::move(task_func));

            if (logging) {
                thread_safe_print() << "Push Task: " << t_id << "\n";
            }
        } // Lock is released HERE. Fast!

        // 2. Signal that an item is ready (Increase Item Count)
        items_available.release();
    }

    void pop_and_run() {
        // 1. Wait for an item (Decrease Item Count)
        items_available.acquire();

        Task current_task(0, 0, nullptr); // Temporary holder

        {
            std::lock_guard<std::mutex> lock(pq_mutex);
            // Move the task out to avoid copying std::function
            // Note: const_cast is needed because priority_queue.top() returns const&
            // Or better: copy it if move is hard with PQ, but std::function is copyable.
            current_task = tasks.top();
            tasks.pop();

            if (logging) {
                thread_safe_print() << "Pop Task: " << current_task.id << "\n";
            }
        } // LOCK RELEASED HERE!

        // 3. Signal that a slot is free (Increase Empty Count)
        // We can do this before OR after running the task.
        // Doing it before allows producers to fill the slot immediately.
        slots_available.release();

        // 4. Run the task (OUTSIDE THE LOCK)
        // Now multiple consumers can run this part in parallel.
        if (current_task.task) {
            current_task.task();
        }
    }
};

int main() {
    MPMCPriority queue(2, false);
    {
        std::vector<std::jthread> producers, consumers;

        // Producers
        for (int i = 0; i < 10; i++) {
            producers.emplace_back([&queue, i]() {
                queue.push(
                        [i]() {
                            // Simulating work
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            thread_safe_print() << "   Exec Task " << i << "\n";
                        },
                        1);
            });
        }

        // Consumers
        for (int i = 0; i < 2; i++) {
            consumers.emplace_back([&queue]() {
                for (int i = 0; i < 5; i++) queue.pop_and_run();
            });
        }
    }
    // jthread destructors wait for threads to finish.
    // Since we push 10 and pop 10, the logic aligns perfectly and terminates.
}

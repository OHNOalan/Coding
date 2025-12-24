#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <semaphore>
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
    std::atomic<int> id;
    std::counting_semaphore<> producer, consumer;
    std::priority_queue<Task> tasks;
    std::mutex rw_mutex;
    bool logging;

public:
    MPMCPriority(int capacity, bool logging = false) : producer(capacity), consumer(0), logging(logging) {}
    void push(std::function<void()> task, int priority) {
        producer.acquire();
        int t_id = id.fetch_add(1, std::memory_order_relaxed);
        {
            std::lock_guard<std::mutex> lock(rw_mutex);
            tasks.emplace(t_id, priority, task);
            if (logging) {
                thread_safe_print() << "Push Task: " << t_id << std::endl;
            }
        }
        consumer.release();
    }
    Task pop() {
        consumer.acquire();
        Task t(0, 0, 0);
        {
            std::lock_guard<std::mutex> lock(rw_mutex);
            t = tasks.top();
            tasks.pop();
            if (logging) {
                thread_safe_print() << "Pop Task: " << t.id << std::endl;
            }
        }
        producer.release();
        t.task();
        return t;
    }
};

int main() {
    MPMCPriority queue(2, false);
    {
        std::vector<std::jthread> producers, consumers;
        for (int i = 0; i < 10; i++) {
            producers.emplace_back([&queue, i]() {
                queue.push(
                        [i]() {
                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            thread_safe_print() << "Task " << i << " executed" << std::endl;
                        },
                        1);
            });
        }
        for (int i = 0; i < 2; i++) {
            consumers.emplace_back([&queue]() {
                for (int i = 0; i < 5; i++) queue.pop();
            });
        }
    }
}

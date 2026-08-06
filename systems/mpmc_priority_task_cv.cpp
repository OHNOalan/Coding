#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>


class thread_safe_print {
    std::stringstream buffer;
    inline static std::mutex output_mutex;

public:
    ~thread_safe_print() {
        std::lock_guard<std::mutex> lock(output_mutex);
        std::cout << buffer.rdbuf();
    }

    template<typename T>
    thread_safe_print &operator<<(const T &msg) {
        buffer << msg;
        return *this;
    }

    thread_safe_print &operator<<(std::ostream &(*func)(std::ostream &) ) {
        buffer << func;
        return *this;
    }
};

std::chrono::steady_clock::time_point get_start_time() {
    static const auto start = std::chrono::steady_clock::now();
    return start;
}

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
    int capacity;
    int id_counter = 0;
    std::condition_variable producer, consumer;

    std::priority_queue<Task> tasks;
    std::mutex pq_mutex;
    bool logging;

public:
    MPMCPriority(int capacity, bool logging = false) : capacity(capacity), logging(logging) {}

    void push(std::function<void()> task_func, int priority) {
        {
            std::unique_lock<std::mutex> lock(pq_mutex);
            int t_id = id_counter++;
            producer.wait(lock, [&]() { return tasks.size() < capacity; });
            tasks.emplace(t_id, priority, std::move(task_func));

            if (logging) {
                thread_safe_print() << "Push Task: " << t_id << "\n";
            }
        }
        consumer.notify_one();
    }

    void pop_and_run() {
        Task current_task(0, 0, nullptr);
        {
            std::unique_lock<std::mutex> lock(pq_mutex);
            consumer.wait(lock, [&]() { return tasks.size() > 0; });
            current_task = tasks.top();
            tasks.pop();

            if (logging) {
                thread_safe_print() << "Pop Task: " << current_task.id << "\n";
            }
        }
        producer.notify_one();
        if (current_task.task) {
            current_task.task();
        }
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
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                            thread_safe_print() << "Exec Task " << i << "\n";
                        },
                        1);
            });
        }

        for (int i = 0; i < 2; i++) {
            consumers.emplace_back([&queue]() {
                for (int i = 0; i < 5; i++) queue.pop_and_run();
            });
        }
    }
}

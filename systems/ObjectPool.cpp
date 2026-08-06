#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <source_location>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

#if defined(__APPLE__)
#include <dlfcn.h>
#include <mach-o/dyld.h>
#endif
#if defined(__linux__)
#include <filesystem>
#endif
#include <unistd.h>

#if __has_include(<execinfo.h>)
#include <execinfo.h>
#define HAS_BACKTRACE 1
#else
#define HAS_BACKTRACE 0
#endif

namespace {

    // ASAN_SYMBOLIZER_PATH 只作用于 Sanitizer 运行时自己的报告；这里用手工调用 llvm-symbolizer
    // 为 backtrace() 得到的地址补全 file:line（需编译带 -g，且 --obj 指向当前可执行文件）。
    std::string self_executable_path() {
#if defined(__APPLE__)
        std::array<char, 4096> buf{};
        uint32_t sz = static_cast<uint32_t>(buf.size());
        if (_NSGetExecutablePath(buf.data(), &sz) == 0) {
            return std::string(buf.data());
        }
#elif defined(__linux__)
        try {
            return std::filesystem::canonical("/proc/self/exe").string();
        } catch (...) {
        }
#endif
        return {};
    }

    std::string llvm_symbolizer_binary() {
        if (const char *p = std::getenv("ASAN_SYMBOLIZER_PATH")) {
            return std::string(p);
        }
        if (const char *p = std::getenv("LLVM_SYMBOLIZER")) {
            return std::string(p);
        }
        static const char *candidates[] = {
                "/opt/homebrew/opt/llvm/bin/llvm-symbolizer",
                "/usr/local/opt/llvm/bin/llvm-symbolizer",
        };
        for (const char *c: candidates) {
            if (::access(c, X_OK) == 0) {
                return std::string(c);
            }
        }
        return {};
    }

    std::string hex_addr(std::uintptr_t addr) {
        std::ostringstream o;
        o << "0x" << std::hex << std::uppercase << addr;
        return o.str();
    }

    std::string popen_read_all(const char *cmd) {
        FILE *fp = ::popen(cmd, "r");
        if (fp == nullptr) {
            return {};
        }
        std::string out;
        char line[512];
        while (std::fgets(line, static_cast<int>(sizeof(line)), fp) != nullptr) {
            out += line;
        }
        (void) ::pclose(fp);
        while (!out.empty() && (out.back() == '\n' || out.back() == '\r')) {
            out.pop_back();
        }
        return out;
    }

    std::string llvm_symbolize_address(std::uintptr_t addr) {
        const std::string tool = llvm_symbolizer_binary();
        const std::string obj = self_executable_path();
        if (tool.empty() || obj.empty()) {
            return {};
        }
        const std::string hx = hex_addr(addr);
        std::array<char, 4096> cmd{};
        std::snprintf(cmd.data(), cmd.size(), "echo %s | \"%s\" --obj=\"%s\" 2>/dev/null", hx.c_str(), tool.c_str(),
                      obj.c_str());
        return popen_read_all(cmd.data());
    }

#if defined(__APPLE__)
    std::string atos_symbolize_address(std::uintptr_t addr) {
        const std::string obj = self_executable_path();
        if (obj.empty()) {
            return {};
        }
        void *const pc = reinterpret_cast<void *>(addr);
        Dl_info inf{};
        if (::dladdr(pc, &inf) == 0 || inf.dli_fbase == nullptr) {
            return {};
        }
        const auto base = reinterpret_cast<std::uintptr_t>(inf.dli_fbase);
        const std::string hx = hex_addr(addr);
        std::array<char, 4096> cmd{};
        std::snprintf(cmd.data(), cmd.size(), "/usr/bin/atos -o \"%s\" -l 0x%llx %s 2>/dev/null", obj.c_str(),
                      static_cast<unsigned long long>(base), hx.c_str());
        return popen_read_all(cmd.data());
    }
#endif

    std::string prettify_frame(std::uintptr_t addr) {
#if defined(__APPLE__)
        std::string a = atos_symbolize_address(addr);
        // atos 失败时常只回显一行十六进制地址；成功通常含 ':' 或 '('
        if (!a.empty() && (a.find(':') != std::string::npos || a.find('(') != std::string::npos)) {
            return a;
        }
#endif
        std::string u = llvm_symbolize_address(addr);
        if (!u.empty() && u.find("??") == std::string::npos) {
            return u;
        }
        return {};
    }

} // namespace

using std::cout;
using std::endl;

struct MyData {
    int id;
    double value;
    std::string tag;

    MyData() : id(0), value(0.0), tag("default") {}
    MyData(int id_, double value_, std::string tag_) : id(id_), value(value_), tag(std::move(tag_)) {}
};

template<typename T>
class IObjectPool {
public:
    virtual ~IObjectPool() = default;
    virtual T *try_acquire_default() = 0;
    virtual bool release(T *obj) = 0;
    virtual std::size_t in_use() const = 0;
    virtual std::size_t capacity() const = 0;
};

// 偏“极限工程化”的固定容量对象池：
// - 固定内存占用（无额外堆分配）
// - 自定义 uint64_t bitmap（替代 std::bitset）
// - intrusive free-list（O(1) acquire/release）
// - 条件变量 + 超时等待接口
// - 关闭 slot 锁：靠严格生命周期顺序 + 全局元数据锁保证安全
// - generation 计数用于调试排查复用问题
template<typename T, std::size_t Capacity>
class FixedObjectPool final : public IObjectPool<T> {
public:
    static_assert(Capacity > 0, "Capacity must be > 0");

    struct Stats {
        std::size_t acquire_success = 0;
        std::size_t acquire_fail = 0;
        std::size_t acquire_timeout = 0;
        std::size_t release_success = 0;
        std::size_t release_fail = 0;
    };

    struct Deleter {
        FixedObjectPool *pool = nullptr;
        void operator()(T *p) const {
            if (pool != nullptr && p != nullptr) {
                (void) pool->release(p);
            }
        }
    };

    struct LeakRecord {
        bool active = false;
        std::thread::id tid{};
        std::chrono::steady_clock::time_point acquired_at{};
        std::source_location where = std::source_location::current();
        std::array<void *, 16> frames{};
        std::size_t frame_count = 0;
    };

    using unique_handle = std::unique_ptr<T, Deleter>;
    static constexpr std::size_t kWordBits = 64;
    static constexpr std::size_t kWords = (Capacity + kWordBits - 1) / kWordBits;

    FixedObjectPool() {
        for (std::size_t i = 0; i < Capacity; ++i) {
            next_free_[i] = i + 1;
            generation_[i] = 0;
        }
        next_free_[Capacity - 1] = kInvalidIndex;
        free_head_ = 0;
    }

    ~FixedObjectPool() override { drain_and_reset(false); }

    FixedObjectPool(const FixedObjectPool &) = delete;
    FixedObjectPool &operator=(const FixedObjectPool &) = delete;

    template<typename... Args>
    T *try_acquire(Args &&...args) {
        return try_acquire_dbg(std::source_location::current(), std::forward<Args>(args)...);
    }

    template<typename Rep, typename Period, typename... Args>
    T *try_acquire_for(const std::chrono::duration<Rep, Period> &timeout, Args &&...args) {
        return try_acquire_for_dbg(timeout, std::source_location::current(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    T *try_acquire_dbg(std::source_location where, Args &&...args) {
        return acquire_impl(std::chrono::nanoseconds::zero(), false, where, std::forward<Args>(args)...);
    }

    template<typename Rep, typename Period, typename... Args>
    T *try_acquire_for_dbg(const std::chrono::duration<Rep, Period> &timeout, std::source_location where,
                           Args &&...args) {
        return acquire_impl(std::chrono::duration_cast<std::chrono::nanoseconds>(timeout), true, where,
                            std::forward<Args>(args)...);
    }

    // 接口兼容：可用于多态使用，但仅支持默认构造。
    T *try_acquire_default() override { return try_acquire(); }

    unique_handle acquire_unique_default() { return unique_handle(try_acquire_default(), Deleter{this}); }

    template<typename... Args>
    unique_handle acquire_unique(Args &&...args) {
        return acquire_unique_dbg(std::source_location::current(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    unique_handle acquire_unique_dbg(std::source_location where, Args &&...args) {
        return unique_handle(try_acquire_dbg(where, std::forward<Args>(args)...), Deleter{this});
    }

    bool release(T *ptr) override {
        if (ptr == nullptr) {
            std::lock_guard<std::mutex> lock(meta_mu_);
            ++stats_.release_fail;
            return false;
        }

        std::lock_guard<std::mutex> lock(meta_mu_);
        const std::size_t idx = index_of(ptr);
        if (idx == Capacity || !bitmap_test(idx)) {
            ++stats_.release_fail;
            return false;
        }
        ptr->~T();
        bitmap_reset(idx);
        generation_[idx]++; // generation 在 release 时推进，便于问题排查
        debug_records_[idx] = LeakRecord{};
        next_free_[idx] = free_head_;
        free_head_ = idx;
        ++stats_.release_success;
        cv_.notify_all();
        return true;
    }

    std::size_t in_use() const override {
        std::lock_guard<std::mutex> lock(meta_mu_);
        return bitmap_count();
    }

    std::size_t capacity() const override { return Capacity; }

    std::size_t free_slots() const {
        std::lock_guard<std::mutex> lock(meta_mu_);
        return Capacity - bitmap_count();
    }

    Stats stats() const {
        std::lock_guard<std::mutex> lock(meta_mu_);
        return stats_;
    }

    void clear() { drain_and_reset(true); }

    template<typename Rep, typename Period>
    bool clear_for(const std::chrono::duration<Rep, Period> &timeout) {
        return drain_and_reset_for(std::chrono::duration_cast<std::chrono::nanoseconds>(timeout), true);
    }

private:
    alignas(T) std::byte storage_[Capacity][sizeof(T)]{};
    std::array<std::uint64_t, kWords> used_words_{};
    std::array<std::size_t, Capacity> next_free_{};
    std::array<std::uint32_t, Capacity> generation_{};
    std::array<LeakRecord, Capacity> debug_records_{};
    std::size_t free_head_ = 0;
    Stats stats_{};
    mutable std::mutex meta_mu_;
    std::condition_variable cv_;
    bool shutting_down_ = false;
    static constexpr std::size_t kInvalidIndex = static_cast<std::size_t>(-1);

    void reset_storage_unlocked() {
        for (std::size_t i = 0; i < Capacity; ++i) {
            next_free_[i] = i + 1;
            used_words_[word_idx(i)] &= ~bit_mask(i);
            debug_records_[i] = LeakRecord{};
        }
        next_free_[Capacity - 1] = kInvalidIndex;
        free_head_ = 0;
    }

    void drain_and_reset(bool reopen_for_acquire) {
        std::unique_lock<std::mutex> lock(meta_mu_);
        shutting_down_ = true;
        cv_.notify_all();
        // Predicate wait handles spurious wakeups by re-checking condition.
        cv_.wait(lock, [this] { return bitmap_count() == 0; });
        reset_storage_unlocked();
        if (reopen_for_acquire) {
            shutting_down_ = false;
            cv_.notify_all();
        }
    }

    bool drain_and_reset_for(std::chrono::nanoseconds timeout, bool reopen_for_acquire) {
        std::unique_lock<std::mutex> lock(meta_mu_);
        shutting_down_ = true;
        cv_.notify_all();
        // Predicate wait_for handles spurious wakeups by re-checking condition.
        const bool drained = cv_.wait_for(lock, timeout, [this] { return bitmap_count() == 0; });
        if (!drained) {
            std::cerr << "[PoolDrainTimeout] still in_use=" << bitmap_count() << "\n";
            std::cerr << dump_outstanding_locked();
            if (reopen_for_acquire) {
                shutting_down_ = false;
                cv_.notify_all();
            }
            return false;
        }
        reset_storage_unlocked();
        if (reopen_for_acquire) {
            shutting_down_ = false;
            cv_.notify_all();
        }
        return true;
    }

    template<typename... Args>
    T *acquire_impl(std::chrono::nanoseconds timeout, bool wait_enabled, std::source_location where, Args &&...args) {
        std::unique_lock<std::mutex> lock(meta_mu_);
        if (shutting_down_) {
            ++stats_.acquire_fail;
            return nullptr;
        }
        if (wait_enabled) {
            // Predicate wait_for handles spurious wakeups by re-checking condition.
            const bool ok =
                    cv_.wait_for(lock, timeout, [this] { return shutting_down_ || free_head_ != kInvalidIndex; });
            if (!ok) {
                ++stats_.acquire_timeout;
                ++stats_.acquire_fail;
                return nullptr;
            }
            if (shutting_down_) {
                ++stats_.acquire_fail;
                return nullptr;
            }
        } else if (free_head_ == kInvalidIndex) {
            ++stats_.acquire_fail;
            return nullptr;
        }

        const std::size_t idx = free_head_;
        free_head_ = next_free_[idx];
        bitmap_set(idx);
        debug_on_acquire_unlocked(idx, where);
        void *place = slot_ptr(idx);
        try {
            T *obj = new (place) T(std::forward<Args>(args)...);
            ++stats_.acquire_success;
            return obj;
        } catch (...) {
            bitmap_reset(idx);
            debug_records_[idx] = LeakRecord{};
            next_free_[idx] = free_head_;
            free_head_ = idx;
            ++stats_.acquire_fail;
            cv_.notify_all();
            throw;
        }
    }

    template<typename Self>
    auto slot_ptr(this Self &&self, std::size_t idx) {
        using raw_self = std::remove_reference_t<Self>;
        using ret_t = std::conditional_t<std::is_const_v<raw_self>, const void *, void *>;
        return static_cast<ret_t>(self.storage_[idx]);
    }

    static std::size_t word_idx(std::size_t idx) { return idx / kWordBits; }
    static std::uint64_t bit_mask(std::size_t idx) { return std::uint64_t{1} << (idx % kWordBits); }

    void bitmap_set(std::size_t idx) { used_words_[word_idx(idx)] |= bit_mask(idx); }
    void bitmap_reset(std::size_t idx) { used_words_[word_idx(idx)] &= ~bit_mask(idx); }
    bool bitmap_test(std::size_t idx) const { return (used_words_[word_idx(idx)] & bit_mask(idx)) != 0; }

    std::size_t bitmap_count() const {
        std::size_t total = 0;
        for (auto word: used_words_) {
            total += static_cast<std::size_t>(std::popcount(word));
        }
        return total;
    }

    static std::size_t capture_backtrace(std::array<void *, 16> &frames) {
#if HAS_BACKTRACE
        const int n = ::backtrace(frames.data(), static_cast<int>(frames.size()));
        return n > 0 ? static_cast<std::size_t>(n) : 0;
#else
        (void) frames;
        return 0;
#endif
    }

    void debug_on_acquire_unlocked(std::size_t idx, std::source_location where) {
        auto &rec = debug_records_[idx];
        rec.active = true;
        rec.tid = std::this_thread::get_id();
        rec.acquired_at = std::chrono::steady_clock::now();
        rec.where = where;
        rec.frame_count = capture_backtrace(rec.frames);
    }

    std::string dump_outstanding_locked() const {
        std::ostringstream oss;
        const auto now = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < Capacity; ++i) {
            if (!bitmap_test(i) || !debug_records_[i].active) {
                continue;
            }
            const auto &rec = debug_records_[i];
            const auto held_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - rec.acquired_at).count();
            oss << "  slot=" << i << " generation=" << generation_[i] << " held_ms=" << held_ms << " tid=" << rec.tid
                << " acquire_at " << rec.where.file_name() << ":" << rec.where.line() << " ("
                << rec.where.function_name() << ")\n";
#if HAS_BACKTRACE
            if (rec.frame_count > 0) {
                char **symbols = ::backtrace_symbols(rec.frames.data(), static_cast<int>(rec.frame_count));
                const std::size_t max_lines = std::min<std::size_t>(rec.frame_count, 8);
                for (std::size_t j = 0; j < max_lines; ++j) {
                    const auto addr = reinterpret_cast<std::uintptr_t>(rec.frames[j]);
                    const std::string pretty = prettify_frame(addr);
                    if (!pretty.empty()) {
                        oss << "    bt[" << j << "] " << pretty << "\n";
                    } else if (symbols != nullptr) {
                        oss << "    bt[" << j << "] " << symbols[j] << "\n";
                    }
                }
                if (symbols != nullptr) {
                    std::free(symbols);
                }
            }
#else
            oss << "    bt: unavailable on this platform/toolchain\n";
#endif
        }
        return oss.str();
    }

    std::size_t index_of(const T *ptr) const {
        const auto base = reinterpret_cast<std::uintptr_t>(slot_ptr(0));
        const auto p = reinterpret_cast<std::uintptr_t>(ptr);
        constexpr std::size_t slot_size = sizeof(T);

        if (p < base) {
            return Capacity;
        }
        const std::size_t diff = static_cast<std::size_t>(p - base);
        if ((diff % slot_size) != 0) {
            return Capacity;
        }
        const std::size_t idx = diff / slot_size;
        if (idx >= Capacity) {
            return Capacity;
        }
        if (reinterpret_cast<const void *>(ptr) != slot_ptr(idx)) {
            return Capacity;
        }
        return idx;
    }
};

int main() {
    cout << "sizeof(MyData) = " << sizeof(MyData) << ", alignof(MyData) = " << alignof(MyData) << endl;
    cout << sizeof(std::string) << endl;

    // 1) 多槽池：典型业务用法
    FixedObjectPool<MyData, 3> multi_pool;
    auto a = multi_pool.acquire_unique(1, 10.5, "A");
    auto b = multi_pool.acquire_unique(2, 20.5, "B");
    auto c = multi_pool.acquire_unique(3, 30.5, "C");
    auto d = multi_pool.acquire_unique(4, 40.5, "D"); // 满池时返回 nullptr
    cout << "[Multi] in_use/free = " << multi_pool.in_use() << "/" << multi_pool.free_slots()
         << ", 4th acquire = " << (d ? "ok" : "nullptr") << endl;
    cout << "[Multi] a.tag = " << (a ? a->tag : "null") << ", b.tag = " << (b ? b->tag : "null") << endl;

    b.reset();
    auto b2 = multi_pool.acquire_unique(22, 220.5, "B-reused");
    cout << "[Multi] after reuse b2.tag = " << (b2 ? b2->tag : "null") << ", in_use = " << multi_pool.in_use() << endl;

    // 3) 工业监控常见统计
    auto st = multi_pool.stats();
    cout << "[Stats] acquire_ok=" << st.acquire_success << ", acquire_fail=" << st.acquire_fail
         << ", acquire_timeout=" << st.acquire_timeout << ", release_ok=" << st.release_success
         << ", release_fail=" << st.release_fail << endl;

    // 2) 单槽池：演示 block 内创建、move 到 block 外继续持有
    FixedObjectPool<MyData, 1> single_pool;
    decltype(single_pool)::unique_handle escaped;
    {
        auto h1 = single_pool.acquire_unique(99, 9.9, "single");
        auto h2 = single_pool.acquire_unique(100, 10.0, "should-fail"); // 单槽，第二次会失败
        cout << "[Single] first = " << (h1 ? "ok" : "null") << ", second = " << (h2 ? "ok" : "null") << endl;
        escaped = std::move(h1); // 把句柄 move 出 block，资源不会在 block 结束时释放
    }
    cout << "[Single] escaped still alive = " << (escaped ? escaped->tag : "null")
         << ", in_use = " << single_pool.in_use() << endl;
    escaped.reset(); // 到这里才会归还
    auto h3 = single_pool.acquire_unique(101, 10.1, "single-reused");
    cout << "[Single] after escaped reset/reacquire = " << (h3 ? h3->tag : "null") << endl;

    // 4) 超时等待示例：先占满，再让另一个线程释放，然后主线程等待获取
    FixedObjectPool<MyData, 1> wait_pool;
    auto hold = wait_pool.acquire_unique(7, 7.7, "hold");
    std::thread releaser([&hold] {
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        hold.reset();
    });
    MyData *waited = wait_pool.try_acquire_for(std::chrono::milliseconds(200), 8, 8.8, "waited");
    cout << "[Wait] acquire_for result = " << (waited ? waited->tag : "timeout") << endl;
    if (waited) {
        (void) wait_pool.release(waited);
    }
    cout << "[Wait timeout leak_demo]" << endl;
    releaser.join();


    // 5) drain 超时 + 高阶追溯示例（会打印未归还句柄来源和调用栈）
    FixedObjectPool<MyData, 1> leak_demo;
    auto leaked = leak_demo.acquire_unique_dbg(std::source_location::current(), 77, 7.7, "leaked");
    const bool drained = leak_demo.clear_for(std::chrono::milliseconds(30));
    cout << "[DrainTimeoutDemo] clear_for drained = " << (drained ? "true" : "false") << endl;
    leaked.reset();
    leak_demo.clear();

    cout << "[Stats] single_pool = " << single_pool.stats().acquire_success << ", " << single_pool.stats().acquire_fail
         << ", " << single_pool.stats().acquire_timeout << ", " << single_pool.stats().release_success << ", "
         << single_pool.stats().release_fail << endl;
    return 0;
}

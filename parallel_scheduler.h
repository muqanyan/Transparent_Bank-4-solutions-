#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t thread_count);
    ~parallel_scheduler();

    template<typename Func, typename... Args>
    auto run(Func&& func, Args&&... args) -> void;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

// Implementation
parallel_scheduler::parallel_scheduler(size_t thread_count) : stop(false) {
    for (size_t i = 0; i < thread_count; ++i)
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
}

parallel_scheduler::~parallel_scheduler() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}

template<typename Func, typename... Args>
auto parallel_scheduler::run(Func&& func, Args&&... args) -> void {
    auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace(task);
    }
    condition.notify_one();
}

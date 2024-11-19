#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);
    ~ThreadPool();

    // Add a new task to the thread pool
    void enqueueTask(const std::function<void()>& task);

private:
    std::vector<std::thread> workers_; // Worker threads
    std::queue<std::function<void()>> tasks_; // Task queue
    std::mutex mtx_; // Mutex for task queue
    std::condition_variable cv_; // Condition variable for worker synchronization
    std::atomic<bool> stop_; // Flag to stop the thread pool

    void workerThread(); // Function executed by worker threads
};

#endif // THREADPOOL_HPP


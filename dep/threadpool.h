#pragma once

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
    // ThreadPool() {}
    ThreadPool(size_t numThreads) : stop(false) {
        // numThreads = n;
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) worker.join();
    }

    /*void start() {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
             });
        }
    }*/

    template<typename F>
    void enqueue(F&& func) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.emplace(std::forward<F>(func));
        }
        condition.notify_one();
    }

    /*void set_numthreads(size_t n) {
        numThreads = n;
        if (workers.size() < n) {
			start();
		}
    }*/
    void wait() {
		for (std::thread& worker : workers) worker.join();
	}
private:
    // size_t numThreads;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stop;
};
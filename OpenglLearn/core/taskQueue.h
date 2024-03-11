//
//  taskQueue.h
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2024/3/11.
//

#ifndef taskQueue_h
#define taskQueue_h

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class TaskQueue {
public:
    static TaskQueue& instance() {
        static TaskQueue queue;
        return queue;
    }
    bool is_empty() {
        int num = _cur_num.load();
        return num == 0;
    }

    void pushTask(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _tasks.push(std::move(task));
            _cur_num.fetch_add(1);
        }
        _condition.notify_one(); // Notify a waiting thread that a new task is available
    }

    void executeTasks() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _condition.wait(lock, [this](){ return !_tasks.empty(); });
                if (_tasks.empty()) // Check if the queue is empty and stop flag is set
                    return;
                task = std::move(_tasks.front());
                _tasks.pop();
            }
            task(); // Execute the task
            _cur_num.fetch_sub(1);
        }
    }

private:
    std::queue<std::function<void()>> _tasks;
    std::mutex _mutex;
    std::condition_variable _condition;
    std::atomic<unsigned int> _cur_num;
};

#endif /* taskQueue_h */

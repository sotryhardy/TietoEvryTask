#pragma once

#include "NotificationQueue.h"
#include "vector"
#include <atomic>
#include <thread>

class TaskSystem
{
private:
    unsigned int                    _count;
    std::vector<std::thread>        _threads;
    std::vector<NotificationQueue>  _queues;
    std::atomic<unsigned int>       _index = 0;

    void run(unsigned int i);

public:
    TaskSystem(int threadCount);
    ~TaskSystem();

    void Start();
    void async_(std::function<void()>&& f);
    void Stop();
    bool busy();

};
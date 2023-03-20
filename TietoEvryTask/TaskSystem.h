#pragma once

#include "NotificationQueue.h"
#include "vector"
#include <atomic>

class TaskSystem
{
private:
    unsigned int                    _count;
    std::vector<std::thread>        _threads;
    std::vector<NotificationQueue>  _q;
    std::atomic<unsigned int>       _index = 0;
    void run(unsigned int i);

public:
    TaskSystem(int threadCount);

    ~TaskSystem();

    template<typename F>
    void async_(F&& f);
};
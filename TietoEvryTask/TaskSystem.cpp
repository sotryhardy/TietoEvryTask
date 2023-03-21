#include "TaskSystem.h"
#include <functional>
#include <iostream>

void TaskSystem::run(unsigned int i)
{
    while (true)
    {
        std::function<void()> f;
        for (int n = 0; n != _count; n++)
        {
            if (_queue[(i + n) % _count].TryPop(f))
            {
                break;
            }
        }
        if (!f && !_queue[i].Pop(f))
        {
            break;
        }
        f();
        std::cout << "function end" << std::endl;
    }
}

TaskSystem::TaskSystem(int threadCount)
    :_queue{ threadCount }
{
    _count = threadCount;
}

TaskSystem::~TaskSystem()
{
    for (auto& thread : _threads)
    {
        thread.join();
    }
}


void TaskSystem::async_(std::function<void()>&& f)
{
     auto i = _index++;

     for (unsigned n = 0; n != _count; n++)
     {
         if (_queue[(i + n) % _count].TryPush(std::forward<std::function<void()>>(f)))
         {
             return;
         }
     }

     _queue[i % _count].Push(std::forward<std::function<void()>>(f));
}
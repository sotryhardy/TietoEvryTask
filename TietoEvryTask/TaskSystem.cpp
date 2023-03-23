#include "TaskSystem.h"
#include <functional>
#include <iostream>

void TaskSystem::run(const unsigned int i)
{
    while (true)
    {
        std::function<void()> f;
        for (int n = 0; n != _count; n++)
        {
            if (_queues[(i + n) % _count].TryPop(f))            // tasks stealing 
            {
                break;
            }
        }
        if (!f && !_queues[i].Pop(f))
        {
            break;
        }
        f();
    }
}

TaskSystem::TaskSystem(int threadCount)
    :_queues{ (long unsigned int)threadCount }
{
    _count = threadCount;
}

TaskSystem::~TaskSystem()
{

}

void TaskSystem::async_(std::function<void()>&& f)
{
     auto i = _index++;

     for (unsigned n = 0; n != _count; n++)
     {
         if (_queues[(i + n) % _count].TryPush(std::forward<std::function<void()>>(f)))         //if some thread now is working with some queue we will try to push into another one queue
         {
             return;
         }
     }

     _queues[i % _count].Push(std::forward<std::function<void()>>(f));
}

void TaskSystem::Start()
{
    _threads.resize(_count);
    for (int i = 0; i < _count; i++) 
    {
        _threads.at(i) = std::thread([i, this]()
            {
                run(i); 
            });
    }
}

void TaskSystem::Stop()
{
    for (auto& queue : _queues)
    {
        queue.Done();
    }
    for (auto& thread : _threads)
    {
        thread.join();
    }
}

bool TaskSystem::busy()
{
    for (auto& queue : _queues)
    {
        if (!queue.empty())
        {
            return true;
        }
    }
    return false;
}
#include "TaskSystem.h"
#include <functional>

void TaskSystem::run(unsigned int i)
{
    while (true)
    {
        std::function<void(std::string pattern, std::string path)> f;
        for (int n = 0; n != _count; n++)
        {
            if (_q[(i + n) % _count].TryPop(f))
            {
                break;
            }
        }
        if (!f && !_q[i].Pop(f))
        {
            break;
        }

        //f();
    }
}

TaskSystem::TaskSystem(int threadCount)
{
    _count = threadCount;
}

template<typename F>
 void TaskSystem::async_(F&& f)
{
     auto i = _index++;

     for (unsigned n = 0; n != _count; n++)
     {
         if (_q[(i + n) % _count].TryPush(std::forward<F>(f)))
         {
             return;
         }
     }

     _q[i % _count].Push(std::forward<F>(f));
}
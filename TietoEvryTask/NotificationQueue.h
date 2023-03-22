#pragma once
#include <deque>
#include <functional>
#include <mutex>

class NotificationQueue
{
private:
    std::deque<std::function<void()>>                                       _queue;
    bool                                                                    _done = false;
    std::mutex                                                              _mutex;
    std::condition_variable                                                 _ready;

public: 
    bool TryPop(std::function<void()>& func);

    bool TryPush(std::function<void()>&& f);

    bool Pop(std::function<void()>& func);

    void Push(std::function<void()>&& f);
    void Done();

    bool empty() const;
};
#include "NotificationQueue.h"

using lock_t = std::unique_lock<std::mutex>;

bool NotificationQueue::TryPop(std::function<void()>& func)
{
    lock_t lock(_mutex, std::try_to_lock);
    if (!lock || _queue.empty())
    {
        return false;
    }

    func = std::move(_queue.front());
    _queue.pop_front();
    return true;
}

bool NotificationQueue::TryPush(std::function<void()>&& f)
{
    {
        lock_t lock(_mutex, std::try_to_lock);
        if (!lock)
        {
            return false;
        }
        _queue.emplace_back(std::forward<std::function<void()>>(f));
    }
    _ready.notify_one();
    return true;
}

bool NotificationQueue::Pop(std::function<void()>& func)
{
    lock_t lock(_mutex);
    while (_queue.empty() && !_done)
    {
        _ready.wait(lock);
    }
    if (_queue.empty())
    {
        return false;
    }
    func = std::move(_queue.front());
    _queue.pop_front();
    return true;
}


void NotificationQueue::Push(std::function<void()>&& f)
{
    {
        lock_t lock(_mutex);
        _queue.emplace_back(std::forward<std::function<void()>>(f));
    }
    _ready.notify_all();
}

void NotificationQueue::Done()
{
    {
        lock_t lock(_mutex);
        _done = true;
    }
    _ready.notify_all();
}

bool NotificationQueue::empty() const
{
    return _queue.empty();
}
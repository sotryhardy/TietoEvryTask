#include "NotificationQueue.h"

using lock_t = std::unique_lock<std::mutex>;

bool NotificationQueue::TryPop(std::function<void(std::string pattern, std::string path)>& func)
{
    lock_t lock(_mutex, std::try_to_lock);
    if (!lock || _q.empty())
    {
        return false;
    }

    func = std::move(_q.front());
    _q.pop_front();
    return true;
}

template<typename F>
bool NotificationQueue::TryPush(F&& f)
{
    {
        lock_t lock(_mutex, std::try_to_lock);
        if (!lock)
        {
            return false;
        }
        _q.emplace_back(std::forward<F>(f));
    }
    _ready.notify_one();
    return true;
}

bool NotificationQueue::Pop(std::function<void(std::string pattern, std::string path)>& func)
{
    lock_t lock(_mutex);
    while (_q.empty() && !_done)
    {
        _ready.wait(lock);
    }
    if (_q.empty())
    {
        return false;
    }
    func = std::move(_q.front());
    _q.pop_front();
    return true;
}

template<typename F>
void NotificationQueue::Push(F&& f)
{
    {
        lock_t lock(_mutex);
        _q.emplace_back(std::forward<F>(f));
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
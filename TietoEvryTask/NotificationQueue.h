#pragma once
#include <deque>
#include <functional>
#include <mutex>

class NotificationQueue
{
private:
    std::deque<std::function<void(std::string pattern, std::string path)>>  _q;
    bool                                                                    _done = false;
    std::mutex                                                              _mutex;
    std::condition_variable                                                 _ready;

public: 
    bool TryPop(std::function<void(std::string pattern, std::string path)>& func);
    template<typename F>
    bool TryPush(F&& f);

    bool Pop(std::function<void(std::string pattern, std::string path)>& func);
    template<typename F>
    void Push(F&& f);
    void Done();
};
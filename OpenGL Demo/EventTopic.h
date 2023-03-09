#pragma once

#include <functional>
#include <unordered_map>

template<typename T>
class EventTopic
{
public:
    long Subscribe(std::function<void(T data)> handler)
    {
        long handlerId = m_nextHandlerId++;
        m_handlers.insert({ handlerId, handler });
        return handlerId;
    }

    void Unsubscribe(long subscription)
    {
        m_handlers.erase(subscription);
    }
    
    void Post(T eventData)
    {
        for (auto const &handler : m_handlers)
        {
            handler.second(eventData);
        }
    }

private:
    std::unordered_map<long, std::function<void(T data)>> m_handlers;
    long m_nextHandlerId = 0;
};

#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include "locker.h"
#include <deque>

// 线程安全的阻塞队列

template<typename T>
class BlockingQueue {
public:
    BlockingQueue() = default;

    void Put(const T &data)
    {
        MutexGuard lock(m_mutex);
        m_queue.push_back(data);
        m_empty_cond.Signal();
    }

    T Pop()
    {
        MutexGuard lock(m_mutex);
        while (m_queue.empty()) {
            m_empty_cond.Wait();
        }
        T data(std::move(m_queue.front()));
        m_queue.pop_front();
        return data;
    }

private:
    std::deque<T>  m_queue;
    Mutex m_mutex;
    Conditon m_empty_cond;
};

#endif  // BLOCKING_QUEUE_H
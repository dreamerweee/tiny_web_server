#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "locker.h"
#include <queue>

class ThreadPool {
public:
    ThreadPool(int thread_cnt = 5);
    ~ThreadPool();

    void AppendWork(void *request);

private:
    static void* Worker(void *args);
    void Run();

private:
    bool m_stop;
    int m_thread_count;   // 线程数量
    pthread_t *m_threads; // 线程数组

    std::queue<void *> m_work_queue;  // 工作队列
    Mutex m_mutex;     // 保护工作队列的互斥锁
    Condition m_cond;  // 工作队列为空时等待的条件变量
}

#endif  // THREAD_POOL_H
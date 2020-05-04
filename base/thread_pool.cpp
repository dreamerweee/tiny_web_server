#include "thread_pool.h"

ThreadPool::ThreadPool(int thread_cnt) :
    m_thread_count(thread_cnt)
{
    assert(thread_cnt > 0);
    m_threads = new pthread_t[m_thread_count];
    assert(m_threads != NULL);
    // 创建线程
    for (int i = 0; i < m_thread_count; ++i) {
        assert(pthread_create(m_threads + i, NULL, Worker, this) == 0);
    }
}
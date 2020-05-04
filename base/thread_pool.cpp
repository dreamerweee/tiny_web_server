#include "thread_pool.h"
#include "log.h"

void* ThreadPool::Worker(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool *>(arg);
    pool->Run();
    return pool;
}

void ThreadPool::Run()
{
    while (!m_stop) {
        MutexGuard lock(m_mutex);
        while (m_task_queue.empty()) {
            m_cond.Wait();
        }
        if (m_stop) {
            LOG_INFO("thread id[0x%x] exit\n", (unsigned)pthread_self());
            pthread_exit(NULL);
        }
        ThreadTask *task = m_task_queue.front();
        m_task_queue.pop();
        if (!task) {
            continue;
        }
        task->cb_func(task->cb_arg);
    }
}

ThreadPool::ThreadPool(int thread_cnt) :
    m_thread_count(thread_cnt), m_stop(false)
{
    assert(thread_cnt > 0);
    m_threads = new pthread_t[m_thread_count];
    assert(m_threads != NULL);
    // 创建线程
    for (int i = 0; i < m_thread_count; ++i) {
        assert(pthread_create(m_threads + i, NULL, Worker, this) == 0);
        assert(pthread_detach(m_threads[i]) == 0);
    }
}
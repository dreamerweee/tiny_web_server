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
        Task *task = m_task_queue.front();
        m_task_queue.pop();
        if (!task) {
            continue;
        }
        task->cb_func(task->cb_arg);
        delete task;
    }
}

ThreadPool::ThreadPool(int thread_cnt) :
    m_thread_count(thread_cnt), m_stop(false)
{
    assert(thread_cnt > 0);
    // 创建线程
    for (int i = 0; i < m_thread_count; ++i) {
        pthread_t *p_thread = new pthread_t;
        m_threads.push_back(p_thread);
        assert(pthread_create(p_thread, NULL, Worker, this) == 0);
        assert(pthread_detach(*p_thread) == 0);
    }
}

ThreadPool::~ThreadPool()
{
    for (int i = 0; i < m_thread_count; ++i) {
        delete m_threads[i];
    }
    if (!m_task_queue.empty()) {
        delete m_task_queue.front();
        m_task_queue.pop();
    }
    m_stop = true;
}

void ThreadPool::AppendWork(TaskCallback func, void *arg)
{
    Task *task = new Task(func, arg);
    MutexGuard lock(m_mutex);
    bool empty_flag = m_task_queue.empty();
    m_task_queue.push(task);
    if (empty_flag) {
        m_cond.Signal();
    }
}

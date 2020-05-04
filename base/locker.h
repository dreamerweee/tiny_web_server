#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>
#include <semaphore.h>
#include <assert.h>

class Mutex {
public:
    Mutex()
    {
        assert(pthread_mutex_init(&m_mutex, NULL) == 0);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }
    
    void Lock()
    {
        assert(pthread_mutex_lock(&m_mutex) == 0);
    }
    void Unlock()
    {
        assert(pthread_mutex_unlock(&m_mutex) == 0);
    }
private:
    pthread_mutex_t m_mutex;
};

// 一个包装类，在stack上建立对象，操作锁
class MutexGuard {
public:
    MutexGuard(Mutex &mutex) : m_mutex(mutex)
    {
        m_mutex.Lock();
    }
    ~MutexGuard()
    {
        m_mutex.Unlock();
    }
private:
    Mutex &m_mutex;
};


class Condition {
public:
    Condition()
    {
        assert(pthread_mutex_init(&m_mutex, NULL) == 0);
        assert(pthread_cond_init(&m_cond, NULL) == 0);
    }
    ~Condition()
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }

    void Wait()
    {
        MutexGuard lock(m_mutex);
        assert(pthread_cond_wait(&m_cond, &m_mutex));
    }
    void Signal()
    {
        assert(pthread_cond_signal(&m_cond) == 0);
    }
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};

#endif  // LOCKER_H
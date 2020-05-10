#include "timer.h"

void Timer::AddTimer(int timeout, TaskCallback func, void *arg)
{
    if (timeout < 0) {
        return ;
    }
    int ticks = 0;  // timeout 时间需要的滴答数
    if (timeout < kSlotTime) {
        ticks = kSlotTime;        
    } else {
        ticks = timeout / kSlotTime;
    }
    int turns = ticks / kTimeSlotNumbers;  // 需要转的圈数
    MutexGuard lock(m_mutex);
    int idx = (m_current_slot + ticks) % kTimeSlotNumbers;  // 插入的时间槽的下标
    TimeWheel *ptw = new TimeWheel(turns, idx, func, arg);
    m_timer[idx].push_back(ptw);
}

void Timer::Tick()
{
    MutexGuard lock(m_mutex);
    std::list<TimeWheel *> tw_lst = m_timer[m_current_slot];
    auto itr = tw_lst.begin();
    while (itr != tw_lst.end()) {
        if (*itr->turn_nums > 0) {
            *itr->turn_nums--;
            ++itr;
        } else {
            *itr->task.cb_func(*itr->task.cb_arg);
            TimeWheel *tmp = *itr;
            itr = tw_lst.erase(itr);
            delete tmp;
        }
    }
    m_current_slot = (++m_current_slot) % kTimeSlotNumbers;
}

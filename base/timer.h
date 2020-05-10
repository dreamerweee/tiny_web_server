#ifndef TIMER_H
#define TIMER_H

#include "locker.h"
#include "task.h"
#include <vector>
#include <list>

/*
* 实现一个时间轮定时器
*/

struct TimeWheel {
    int turn_nums;  // 时间轮转多少圈后生效
    int time_slot;  // 该定时器所在的时间轮上的槽位
    Task task;   // 任务回调

    TimeWheel(int tn, int ts, TaskCallback func, void *arg)
        : turn_nums(tn), time_slot(ts), task(func, arg)
    { }
};

class Timer {
public:
    Timer() : m_current_slot(0)
    { }
    ~Timer();
    void AddTimer(int timeout, TaskCallback func, void *arg);
    // void DelTimer(TimeWheel *time_wheel);
    void Tick();

private:
    static const int kTimeSlotNumbers = 60;  // 时间轮包含60个槽
    static const int kSlotTime = 1;  // 每1s转到一次，即槽间隔为1s
    
    int m_current_slot;   // 时间轮所在的当前槽位
    std::vector<std::list<TimeWheel *>> m_timer;  // 保存时间轮
    Mutex m_mutex;
};

#endif  // TIMER_H
#ifndef TASK_H
#define TASK_H

typedef void (*TaskCallback) (void *arg);

struct Task {
    TaskCallback cb_func;
    void *cb_arg;
    Task(TaskCallback func, void *arg)
        : cb_func(func), cb_arg(arg)
    { }
};


#endif  // TASK_H
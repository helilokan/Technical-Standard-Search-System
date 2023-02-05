#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "Thread.h"
#include "TaskQueue.h"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

using Task = function<void()>;

class ThreadPool
{
    friend class WorkThread;
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task &&ptask);
    Task getTask();
private:
    void threadFunc();

private:
    size_t _threadNum;
    size_t _queSize;
    bool _isExit;
    TaskQueue _taskQue;
    vector<unique_ptr<Thread>> _threads;
};
#endif



#ifndef _QUEUE_H
#define _QUEUE_H

#include "NoCopyable.h"
#include "MutexLock.h"
#include "Condition.h"
#include <functional>
#include <queue>

using std::function;
using std::queue;

using Elem = function<void(unsigned long)>;

class TaskQueue
: public NoCopyable
{
public:
    TaskQueue(size_t);
    ~TaskQueue()
    {

    }
    bool empty() const;
    bool full() const;
    void push(Elem && value);
    Elem pop();

    // 将所有_notEmpty唤醒
    void wakeup();

private:
    size_t _queSize;
    queue<Elem> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;// 用于作为唤醒时的标记位
};

#endif

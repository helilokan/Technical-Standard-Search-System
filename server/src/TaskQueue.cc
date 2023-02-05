#include "../include/TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true)
{

}

bool TaskQueue::empty() const
{
    return _que.size() == 0;
}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
}

void TaskQueue::push(Elem && value)
{
    MutexLockGuard autolock(_mutex);// 栈对象，对象析构自动释放锁

    while(full())// 用while不用if防止虚假唤醒
    {
        _notFull.wait();
    }

    _que.push(value);
    _notEmpty.notify();
}

Elem TaskQueue::pop()
{
    MutexLockGuard autolock(_mutex);

    if(empty())
    {
        _notEmpty.wait();
    }

    if(_flag)
    {
        Elem tmp = _que.front();
        _que.pop();
        _notFull.notify();
        return tmp;
    }
    else
    {
        return nullptr;
    }
}

// 唤醒所有阻塞在_notEmpty的线程
void TaskQueue::wakeup()
{
    _notEmpty.notifyall();
    _flag = false;
}

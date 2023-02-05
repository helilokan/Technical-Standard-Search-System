#ifndef _MUTEX_H
#define _MUTEX_H

#include "NoCopyable.h"
#include <pthread.h>

class MutexLock
: public NoCopyable
{
public:
    MutexLock();
    ~MutexLock();
    void Lock();
    void unLock();
    pthread_mutex_t *getMutexLockPtr()
    {
        return &_mutex;
    }
private:
    pthread_mutex_t _mutex;

};

// 利用RAII的思想实现锁的自动释放
class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &mutex)
    : _mutex(mutex)
    {
        _mutex.Lock();
    }
    ~MutexLockGuard()
    {
        _mutex.unLock();
    }
private:
    MutexLock &_mutex;
};

#endif


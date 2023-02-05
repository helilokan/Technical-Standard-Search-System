#ifndef _COND_H
#define _COND_H

#include "NoCopyable.h"
#include <pthread.h>

class MutexLock;// 前向声明，防止头文件重复包含可能报错

class Condition
: public NoCopyable
{
public:
    Condition(MutexLock &);
    ~Condition();
    void wait();
    void notify();
    void notifyall();

private:
    pthread_cond_t _cond;
    MutexLock &_mutex;
};
#endif


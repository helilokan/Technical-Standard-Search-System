#include "../include/Condition.h"
#include "../include/MutexLock.h"// 要调用里面的函数，所以需要引入头文件

Condition::Condition(MutexLock &mutex)
: _mutex(mutex)
{
    pthread_cond_init(&_cond, nullptr);
}

Condition::~Condition()
{
    pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
    pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
}

void Condition::notify()
{
    pthread_cond_signal(&_cond);
}

void Condition::notifyall()
{
    pthread_cond_broadcast(&_cond);
}

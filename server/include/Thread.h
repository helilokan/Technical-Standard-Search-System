#ifndef _THREAD_H
#define _THREAD_H

#include "NoCopyable.h"
#include <pthread.h>
#include <functional>

using std::function;
using std::bind;

using ThreadCallback = function<void(unsigned long)>;

class CacheManager;

class Thread
: public NoCopyable
{
public:
    Thread(ThreadCallback &&, CacheManager * cach);
    ~Thread();
    void start();
    void join();

private:
    static void *threadFunc(void *);

private:
    static CacheManager * _cach;
    pthread_t _thid;
    bool _isRuning;
    ThreadCallback _cb;
};
#endif


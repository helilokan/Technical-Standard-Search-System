#ifndef _THREAD_H
#define _THREAD_H

#include "NoCopyable.h"
#include <pthread.h>
#include <functional>

using std::function;
using std::bind;

using ThreadCallback = function<void()>;

class Thread
: public NoCopyable
{
public:
    Thread(ThreadCallback &&);
    ~Thread();
    void start();
    void join();

private:
    static void *threadFunc(void *);

private:
    pthread_t _thid;
    bool _isRuning;
    ThreadCallback _cb;
};
#endif


#include "../include/Thread.h"
#include "../include/CacheManager.h"
#include <stdio.h>

__thread int addr = 1;

CacheManager *Thread::_cach = nullptr;

Thread::Thread(ThreadCallback && cb, CacheManager *cach)
: _thid(0)
, _isRuning(false)
, _cb(cb)
{
    _cach = cach;
}

Thread::~Thread()
{
    if(_isRuning)
    {
        // 线程自动分离
        pthread_detach(_thid);
    }
}

void Thread::start()
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    /* cout << "Tid =  " << _thid << endl */
    /*      << "addr = " << (unsigned long)&addr << endl; */
    if(ret)
    {
        perror("pthread_create");
        return;
    }
    _isRuning = true;
}

void Thread::join()
{
    if(_isRuning)
    {
        pthread_join(_thid, nullptr);
        _isRuning = false;
    }
}

void *Thread::threadFunc(void *arg)
{
    
    _cach->initCache((unsigned long)&addr);
    
    Thread *pth = static_cast<Thread*>(arg);// 强制转换
    if(pth)
    {
        pth->_cb((unsigned long) &addr);// 执行回调函数
    }
    pthread_exit(nullptr);
}

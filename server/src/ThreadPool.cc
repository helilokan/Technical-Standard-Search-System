#include "../include/ThreadPool.h"
#include "../include/CacheManager.h"
#include <unistd.h>


ThreadPool::ThreadPool(size_t threadNum, size_t queSize, CacheManager *cach)
: _cach(cach)
, _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{
    _threads.reserve(threadNum);
}

void ThreadPool::start()
{
    for(size_t idx = 0; idx < _threadNum; ++idx)
    {
        using namespace std::placeholders;
        unique_ptr<Thread> up(new Thread(std::bind(&ThreadPool::threadFunc, this, _1), _cach));
        _threads.push_back(std::move(up));
    }
    for(auto &th : _threads)
    {
        th->start();// 调用Thread的start函数，
    }
}

void ThreadPool::stop()
{
    // 任务队列还有任务线程就不关闭
    while(!_taskQue.empty())
    {
        sleep(1);
    }
    _isExit = true;
    // 防止线程卡死，退出时唤醒所有线程
    _taskQue.wakeup();
    for(auto &th : _threads)
    {
        th->join();
    }
}

void ThreadPool::addTask(Task &&ptask)
{
    if(ptask)
    {
        _taskQue.push(std::move(ptask));
    }
}

Task ThreadPool::getTask()
{
    return _taskQue.pop();
}

ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        stop();
    }
}

void ThreadPool::threadFunc(unsigned long addr)
{
    while(!_isExit)
    {
        Task taskcb = getTask();
        if(taskcb)
        {
            taskcb(addr);
        }
    }
}


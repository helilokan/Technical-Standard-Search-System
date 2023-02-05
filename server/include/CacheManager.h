#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__
#include "Thread.h"
#include "MutexLock.h"
#include "LRUcache.h"
#include <vector>
#include <functional>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <set>

using std::vector;
using std::multiset;
using std::map;
using std::cout;
using std::endl;

bool operator<(const Node &rhs, const Node &lhs);

class CacheManager
{
public:
    static CacheManager * getInstance();
    //从磁盘文件中读取缓存信息初始化每个线程的cache
    void initCache(unsigned long address);
    
    void show();

    string findInCache(unsigned long addr, string key);

    void addInCache(unsigned long addr, string key, vector<string> val);
    //定时更新所有的缓存
    void periodicUpdateCaches();
    void writeToFile(const string & filename);

private:
    
    class AutoRelease
    {
    public:
        AutoRelease()
        {

        }

        ~AutoRelease()
        {
            if(_pInstance)
            {
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };

    CacheManager()
    : _mutex()
    {

    }
    ~CacheManager()
    {
        if(_pInstance)
        {
            for(auto & it : _cacheList)
            {
                delete it;
            }
        }
    }
private:
    static CacheManager *_pInstance;
    static AutoRelease _ar;
    MutexLock _mutex;
    vector<LRUCache*> _cacheList;// 存储每个cache
    map<unsigned long, int> _threadCache;// 用于对应不同线程的cache
    multiset<Node> _sumnode;
};

#endif


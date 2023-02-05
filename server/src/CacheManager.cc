#include "../include/CacheManager.h"
#include "../include/Configuration.h"

bool operator<(const Node &rhs, const Node &lhs)
{
    // 按照runtime从大到小排序
    return lhs.runtime >= rhs.runtime;
}

CacheManager* CacheManager::_pInstance = getInstance();
CacheManager::AutoRelease CacheManager::_ar;

CacheManager * CacheManager::getInstance()
{
    if(nullptr == _pInstance)
    {
        _pInstance = new CacheManager();
    }
    return _pInstance;
}

void CacheManager::show()
{
   cout << "当前缓存池展示:" << endl;
   cout << "当前缓存个数：" << _cacheList.size() << endl;
   for(int i = 0; i < _cacheList.size(); ++i)
   {
       _cacheList[i]->print();
   }
   cout << endl << endl;
}

//从磁盘文件中读取缓存信息
void CacheManager::initCache(unsigned long address)
{
    // 会修改缓存池，加锁操作
    MutexLockGuard autolock(_mutex);

    LRUCache *lru = new LRUCache();
    Configuration * pconf = Configuration::getInstance();
    map<string, string> tmp = pconf ->getConfigMap();
    lru->readFromFile(tmp["cache1"]);
    _cacheList.push_back(lru);
    int cacheId = _cacheList.size();
    
    /* cout << "address:" << address << endl; */
    /* cout << "cacheId:" << cacheId - 1 << endl; */
    
    /* _cacheList[cacheId - 1]->print(); */
    /* show(); */

    _threadCache.insert({address,cacheId - 1});
}

string CacheManager::findInCache(unsigned long addr, string key)
{
    // 获取当前线程对应的缓存的下标
    cout << "address" << addr << endl;
    int idx = _threadCache[addr];   
    cout << "find in cache No. " << idx << endl;
    cout << "key = " << key << endl;
    cout << "size of key = " << key.size() << endl;
    return _cacheList[idx]->findElement(key);
}

void CacheManager::addInCache(unsigned long addr, string key, vector<string> val)
{
    // 会修改缓存池，加锁操作
    MutexLockGuard autolock(_mutex);

    int idx = _threadCache[addr];
    cout << "cache No. " << idx << " update" << endl;
    _cacheList[idx]->addElement(key, val);
}

//定时更新所有的缓存
void CacheManager::periodicUpdateCaches()
{
    /* cout << endl << "定时唤醒" << endl; */
    
    // 会修改缓存池，加锁操作
    MutexLockGuard autolock(_mutex);

    for(size_t i = 0; i < _cacheList.size(); ++i)
    {
        /* cout << "No. " << i */
        /*      << " pending size = " << _cacheList[i]->_pendingUpdateList.size() << endl; */
        if(_cacheList[i]->_pendingUpdateList.empty())
        {
            continue;
        }
        for(auto elem: _cacheList[i]->_pendingUpdateList)
        {
            _sumnode.insert(elem);
        }
        _cacheList[i]->_pendingUpdateList.clear();
    }
    Configuration * pconf = Configuration::getInstance();
    map<string, string> tmp = pconf->getConfigMap();
    //写到Cache1的文件
    if(!_sumnode.empty())
    {
        writeToFile(tmp["cache1"]);
    }
    
    for(size_t i = 0; i < _cacheList.size(); ++i)
    {
        _cacheList[i]->readFromFile(tmp["cache1"]);
    }

    if(_sumnode.size() > 1000)
    {
        auto it = _sumnode.begin();
        for(int i = 0; i < 1000; ++i)
        {
            ++it;
        }
        _sumnode.erase(it, _sumnode.end());
    }
}



void CacheManager::writeToFile(const string & filename)
{
    /* cout << "filename = " << filename << endl; */
    ofstream of(filename);
    int idx = 0;
    for(auto i : _sumnode)
    {
        of << i.key << " " << i.runtime << " ";
        for(auto & elem : i.val)
        {
            of << elem << " ";
        }
        of << "\n";
        
        ++idx;
        if(idx >= 1000)
        {
            break;
        }
    }
    of.close();
}


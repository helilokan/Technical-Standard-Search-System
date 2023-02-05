#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <iostream>  
#include <map>  
#include <list>  
#include <utility>  
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>

using std::pair;
using std::map;
using std::string;
using std::list;
using std::make_pair;
using std::cout;
using std::endl;
using std::unordered_map;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::time_t;
using std::vector;

struct Node {
    string key;
    vector<string> val;
    time_t runtime;
    Node(string _key, vector<string> _val) 
    : key(_key)
    , val(_val) 
    {
        runtime = time(nullptr);
    }
    Node(string _key, vector<string> _val, time_t _runtime) 
    : key(_key), val(_val) , runtime(_runtime)
    {}
}; // 模拟内存虚拟页

class LRUCache {
public:

    //等待更新的节点信息
    list<Node> _pendingUpdateList;
    
    void print();
    //访问某个页表
    string findElement(string key);

    void addElement(string key, vector<string> val);
    void addElement(string key, vector<string> val, time_t runtime);
    void readFromFile(const string & filename);

private:
    // 存储页表的数量
    size_t capacity = 1000; 
    //页表调度队列
    list<Node> _resultList; 
    //采用 hashTable 进行查找
    unordered_map<string, list<Node>::iterator> _hashMap;
};

#endif

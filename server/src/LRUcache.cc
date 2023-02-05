#include "../include/LRUcache.h"

void LRUCache::print()
{
    cout << "_hashMap.size() = " << _hashMap.size() << endl;
    cout << "_resultList.size() = " << _resultList.size() << endl;
    cout << "resultList:" << endl;
    auto it = _resultList.begin();
    for(auto &elem : _resultList)
    {
        cout << "key of resultList: " << elem.key << endl;
        /* printf("对应的 hashmap 中的链表结点是%p\n", &elem); */
        /* printf("对应的 hashmap 中的链表结点是%p\n", it++); */
        cout << "value: ";
        for(auto &e : elem.val)
        {
            cout << e << " ";
        }
        cout << endl;

        cout << "key: " << elem.key << endl
             << "runtime: " << elem.runtime << endl;
    }
    cout << "hashMap:" << endl;
    for(auto &elem : _hashMap)
    {
        cout << "key of hashmap: " << elem.first << endl;
        /* printf("对应的 hashmap 中的链表结点是%p\n", elem.second); */
        
        cout << "value: ";
        for(auto &e : elem.second->val)
        {
            cout << e << " ";
        }
        cout << endl;
        
        cout << "key: " << elem.second->key << endl;
        cout << "rumtime:" << elem.second->runtime << endl;
    }

    cout << endl << endl;
 
}

string LRUCache::findElement(string key) 
{ 
    cout << "我要找的词是:" << key <<  endl;
    //访问某个页表
    if(_hashMap.find(key) == _hashMap.end()) 
    {
        return "err"; // 产生中断, 调用setPage
    }

    // 将key移动到链表的第一个位置, 更新_hashMap
    _resultList.splice(_resultList.begin(), _resultList, _hashMap[key]);
    _hashMap[key] = _resultList.begin();

    // 获取结果
    vector<string> vec = _hashMap[key]->val;
    string tmp;
    // 判断是否为网页
    if('2' == key[0])
    {
        if(vec.empty())
        {
            return "err";
        }
        tmp = vec[0];
        for(int i = 1; i < vec.size(); ++i)
        {
            if("[标题]" == vec[i])
            {
                tmp += "\n" + vec[i] + " ";
                tmp += vec[++i] + "\n";
            }
            else if("[网址]" == vec[i])
            {
                tmp += vec[i] + " ";
                tmp += vec[++i] + "\n";
            }
            else
            {
                tmp += vec[i] + " ";
            }
            
        }
   
    }
    
    else
    {
        for(int i = 0; i < vec.size(); ++i)
        {
            tmp += vec[i] + " ";
        }
    }
    return tmp; // 返回目标值
}

void LRUCache::addElement(string key, vector<string> val) 
{
    if(_hashMap.find(key) == _hashMap.end()) 
    {
        if(_resultList.size() == capacity) 
        { 
            // 淘汰最后一个
            _hashMap.erase(_resultList.back().key);
            _resultList.pop_back();
        }
        // 将新的页面添加进来
        _resultList.push_front(Node(key, val));
        _hashMap[key] = _resultList.begin();
        // 新加入的页面加入待更新队列
        _pendingUpdateList.push_front(Node(key, val));
    } 
    else 
    {
        // 目标页面已经在内存中, 更新结点即可
        _hashMap[key]->val = val;
        _resultList.splice(_resultList.begin(), _resultList, _hashMap[key]);
        _hashMap[key] = _resultList.begin();
    }
}

void LRUCache::addElement(string key, vector<string> val, time_t runtime) 
{
    if(_hashMap.find(key) == _hashMap.end()) 
    {
        /* cout << "11111" << endl; */
        if(_resultList.size() == capacity) 
        { 
            // 淘汰最后一个
            _hashMap.erase(_resultList.back().key);
            _resultList.pop_back();
        }
        //将新的页面添加进来
        Node tmp_node(key, val, runtime);
        _resultList.emplace_front(tmp_node);
        /* _pendingUpdateList.push_front(Node(key, val, runtime)); */
        _hashMap[key] = _resultList.begin();
        /* printf("%s 对应的 resultlist 中的链表结点是%p\n", key.c_str(), _resultList.begin()); */
        /* printf("%s 对应的 hashmap 中的链表结点是%p\n", key.c_str(), _hashMap[key]); */
    } 
    else 
    {
        /* cout << "2222" << endl; */
        // 目标页面已经在内存中, 更新结点即可
        _hashMap[key]->val = val;
        _hashMap[key]->runtime = runtime;
        _resultList.splice(_resultList.begin(), _resultList, _hashMap[key]);
        _hashMap[key] = _resultList.begin();
    }
}

void LRUCache::readFromFile(const string & filename)
{
    ifstream ifs(filename);
    if(!ifs.good())
    {
        cout << "readFromFile open file error" << endl;
        return;
    }
    string line;
    while(getline(ifs, line))
    {
        istringstream iss(line);
        string key;
        vector<string> value;
        time_t runtime;
        string tmp;

        iss >> key;

        iss >> runtime;

        while(iss >> tmp)
        {
            value.push_back(tmp); 
        }
        /* cout << "value size = " << value.size() << endl; */
        addElement(key,value,runtime);
    }
    /* cout << "插完了，show一下" << endl; */
    /* print(); */
}



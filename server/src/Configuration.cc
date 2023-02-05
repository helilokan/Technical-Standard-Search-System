#include "../include/Configuration.h"
#include "../include/DirScanner.h"
#include <fstream>
#include <sstream>
#include <iostream>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

Configuration* Configuration::_pInstance = getInstance();
Configuration::AutoRelease Configuration::_ar;// 默认初始化
string Configuration::_filepath = "../conf";

Configuration *Configuration::getInstance()
{
    if(nullptr == _pInstance)
    {
        _pInstance = new Configuration();
    }
    return _pInstance;
}

// 读取配置文件
void Configuration::readConfig(const string& filepath)
{
    ifstream ifs(filepath);
    if(!ifs.good())
    {
        cout << "ifstream open file error" << endl;
        return;
    }
    string line;
    while(getline(ifs, line))
    {
        istringstream iss(line);
        string name;
        string path;
        iss >> name >> path;
        _configMap[name] = path;
    }
}

// 读取停用词
void Configuration::readStopword(const string& filepath)
{
    ifstream ifs(filepath);
    if(!ifs.good())
    {
        cout << "ifstream open file error" << endl;
        return;
    }
    string line;
    while(getline(ifs, line))
    {
        _stopWordList.insert(line);
    }

}


map<string, string> &Configuration::getConfigMap()
{
    // 读取配置文件
    DirScanner dir;
    dir(_filepath);
    for(auto & path : dir.files())
    {
        readConfig(path);
    }

    return _configMap;
}

unordered_set<string> Configuration::getStopWordList()
{
    // 读取停用词
    string stopword_path = "../data/stopWord";
    DirScanner dir;
    dir(stopword_path);
    for(auto & path : dir.files())
    {
        readStopword(path);
    }

    return _stopWordList;
}


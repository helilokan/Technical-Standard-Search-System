#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>
#include <unordered_set>

using std::string;
using std::map;
using std::unordered_set;

// 设计成单例模式，保证其他类不会对其进行修改
class Configuration
{
    /* friend class AutoRelease; */
public:
    static Configuration* getInstance();
    void readConfig(const string& filepath);
    void readStopword(const string& filepath);
    map<string, string> &getConfigMap();
    unordered_set<string> getStopWordList();
    
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

    Configuration() {}
    ~Configuration() {}
private:
    static Configuration *_pInstance;
    static AutoRelease _ar;
    static string _filepath;
    map<string, string> _configMap;
    unordered_set<string> _stopWordList;
};
#endif


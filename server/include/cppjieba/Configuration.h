#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>
#include <set>

using std::string;
using std::map;
using std::set;

// 设计成单例模式，保证其他类不会对其进行修改
class Configuration
{
    friend class AutoRelease;
public:
    static Configuration* getInstance();
    void readConfig(const string& filepath);
    void readStopword(const string& filepath);
    map<string, string> &getConfigMap();
    set<string> &getStopWordList();
    
private:
    class AutoRelease
    {
    public:
        AutoRelease()
        {

        }

        ~AutoRelease()
        {
            if(Configuration::_pInstance)
            {
                delete Configuration::_pInstance;
                Configuration::_pInstance = nullptr;
            }
        }
    };

    Configuration() 
    {
        _filepath = "../conf";
    }
    ~Configuration() {}
private:
    static Configuration *_pInstance;
    static AutoRelease _ar;
    string _filepath;
    map<string, string> _configMap;
    set<string> _stopWordList;
};
#endif


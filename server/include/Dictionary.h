#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <queue>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <utility>

using std::priority_queue;
using std::pair;
using std::string;
using std::vector;
using std::set;
using std::unordered_map;

struct CandidateResult
{
    CandidateResult(string word, int freq, int dist)
    : _word(word)
    , _freq(freq)
    , _dist(dist)
    {}
    
    string _word;
    int _freq;
    int _dist;
};

bool operator<(const CandidateResult &lhs, const CandidateResult &rhs);

class Configuration;

class Dictionary
{
public:
    static Dictionary* getInstance();
    priority_queue<CandidateResult> doQuery(const string& keyWord);
    void readDict();
    void readIndex();
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

    Dictionary(){}
    ~Dictionary() {}

private:
    size_t _ch_length;
    vector<pair<string, int>> _dict;
    unordered_map<string, set<int>> _index;
    static Dictionary *_pInstance;
    static AutoRelease _ar;
    static Configuration *_conf;
};
#endif


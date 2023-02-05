#include "../include/Configuration.h"
#include "../include/CppJieba.h"
#include <unordered_map>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::make_pair;
using std::pair;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

CppJieba::CppJieba(Configuration *conf)
: _jieba(DICT_PATH, HMM_PATH, "")
, _conf(conf)
{
    cout<<"jiba init"<<endl;
}

CppJieba::~CppJieba()
{
    cout<<"~jiba"<<endl;
}

vector<pair<string, int>> CppJieba::cut(const string &sentence)
{
    vector<pair<string, int>> ret;
    vector<string> allwords;
    _jieba.Cut(sentence, allwords, true);

    vector<string> Erase_StopWord;
    unordered_set<string> stop_word = _conf->getStopWordList();
    unordered_map<string, int> word_query;

    for (auto &it : allwords)
    {
        if (stop_word.find(it) != stop_word.end())
        {
            continue;
        }
        else
        {
            Erase_StopWord.push_back(it);
        }
    }

    for (auto &it : Erase_StopWord)
    {
        unsigned char temp = it[0];
        
        // 跳过英文(网页库的分次重写了分词，这个只用于建立中文关键词词典)
        if(temp < 128)
        {
            continue;
        }
        word_query[it]++;    
    }
    for (auto &it : word_query)
    {
        ret.push_back(make_pair(it.first, it.second));
    }
    return ret;
}

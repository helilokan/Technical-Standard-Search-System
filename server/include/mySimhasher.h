#ifndef __MYSIMHASHER_H__
#define __MYSIMHASHER_H__

//this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN

#include "../include/simhash/Simhasher.hpp"

using namespace simhash;

class Configuration;

class mySimhasher
{
public:
    mySimhasher(Configuration *conf);
    ~mySimhasher();
    uint64_t calcSimhash(const string &text);
    unordered_map<string, int> cut(const string &text); 
private:
    const static int TOPK_NUMBER;
    Configuration *_conf;
    unordered_set<string> _stopWordList;
    Simhasher _simhasher;    
    Jieba _jieba;
};
#endif


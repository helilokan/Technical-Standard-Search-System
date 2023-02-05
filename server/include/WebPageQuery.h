#ifndef __WEBPAGEQUERY_H__
#define __WEBPAGEQUERY_H__

#include "mySimhasher.h"
#include "tinyxml2.h"
#include "Configuration.h"

#include <map>
#include <vector>
#include <set>
#include <unordered_map>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>
#include <codecvt>
#include <fstream>

using std::cerr;
using std::map;
using std::pair;
using std::set;
using std::unordered_map;
using std::vector;

using namespace tinyxml2;

class WebPageQuery
{
public:
    WebPageQuery(Configuration *conf);
    vector<string> doQuery(const string &QueryString); //返回查询结果 相似的前10的网页
    void loadLibrary();
    void findKeyPageWeb();
    void contAllKeyWordPageID();
    void coutQueryKeyWord();
    void coutCosWeight();
    void CosPageWebRank();
    void makeAbs();
    void clear();
    size_t getIdx(size_t idx);
    size_t nBytes(char ch);

    void bulitBaseVector(); //计算基准向量

private:
    mySimhasher _smhs;
    Configuration *_conf;
    unordered_map<int, pair<int, int>> _offsetLib; //偏移库
    unordered_map<string, set<pair<int, double>>> _invertIndexTable;
    vector<vector<int>> _pagesIdQueryed;
    vector<int> _AllKeyWordPageID;
    map<int, vector<double>> _CosWeight;
    vector<pair<int, double>> _CosPageWebRank;
    unordered_map<string, int> _freq; //查询关键词的词频
    set<string> _keyWord;             //在倒排索引表中出现的词
    map<string, double> _BaseVector;
    vector<string> _QueryKeyWord;
    vector<string> _Abs;
};

#endif

#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include "DirScanner.h"
#include "WebPage.h"
#include <utility>
#include <set>

using std::set;
using std::pair;

class mySimhasher;

class PageLibPreprocessor
{
public:
    PageLibPreprocessor(mySimhasher &mysh);
    void doProcess();
    void buildInvertIndexTable();
    void storeOnDisk();
private:
    //分词对象 
    mySimhasher &_mysh;
    //网页库容器对象
    set<WebPage> _pageLib;
    //网页偏移库
    unordered_map<int, pair<int, int> > _offsetLib;
    //倒排索引
    unordered_map<string, vector<pair<int, double> > >  _invertIndexTable;
};

#endif


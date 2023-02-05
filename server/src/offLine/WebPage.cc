#include "../include/WebPage.h"
#include "../include/mySimhasher.h"
#include "../include/Configuration.h"
#include <iostream>

using std::cout;
using std::endl;


//this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN


using namespace simhash;

using std::ostream_iterator;

bool operator==(const WebPage &lhs, const WebPage &rhs)
{
    
    return Simhasher::isEqual(lhs._docSimhash, rhs._docSimhash);
}

bool operator<(const WebPage &lhs, const WebPage &rhs)
{
    if(lhs == rhs)
    {
        return false;
    }
    return lhs._docId < rhs._docId;
}

const int WebPage::TOPK_NUMBER = 20;

WebPage::WebPage(string & doc, int docId, RssItem &rss, mySimhasher &mysh)
: _mysh(mysh)
, _doc(doc)
, _docId(docId)
, _docTitle(rss.title)
, _docUrl(rss.url)
, _docContent(rss.description + rss.content)
{
    processDoc();
}

int WebPage::getDocId()
{
    return _docId;
}

string WebPage::getDoc()
{
    return _doc;
}

unordered_map<string, int> &WebPage::getWordsMap()
{
    return _wordsMap;
}

void WebPage::processDoc()
{
    // 1. 分词，计算词频并存入_wordsMap
    _wordsMap = _mysh.cut(_docContent);

    // 2. 计算文章的simhash值
    _docSimhash = _mysh.calcSimhash(_docContent);
    
    /* cout << "simhash:" << _docSimhash << endl; */
}

void WebPage::calcTopK()
{
    Configuration * pconf = Configuration::getInstance();
    map<string, string> tmp = pconf->getConfigMap(); 
    Simhasher simhasher(tmp["dictPath"], tmp["modelPath"], tmp["idfPath"], tmp["stopWords"]);
    vector<pair<string, double>> res;
    simhasher.extract(_docContent, res, TOPK_NUMBER);
    for(auto & elem : res)
    {
        _topWords.push_back(elem.first);
    }
    ostream_iterator<string> osi(cout, " ");
    auto begin = _topWords.begin();
    auto end = _topWords.end();
    copy(begin, end, osi);
}

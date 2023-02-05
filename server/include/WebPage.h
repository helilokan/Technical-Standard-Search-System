#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include "RssParses.h"
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map; 

class mySimhasher;

class WebPage
{
    friend bool operator==(const WebPage &lhs, const WebPage &rhs);
    friend bool operator<(const WebPage &lhs, const WebPage &rhs);
    friend class PageLibPreprocessor;
public:
    WebPage(string & doc, int docId, RssItem &rss, mySimhasher &mysh);
    int getDocId();
    string getDoc();
    unordered_map<string, int> &getWordsMap();
    void processDoc();
    void calcTopK();

private:
    const static int TOPK_NUMBER;
    mySimhasher &_mysh;
    string _doc;
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    string _docSummary;
    uint64_t _docSimhash;
    vector<string> _topWords;
    unordered_map<string, int> _wordsMap;
};
#endif


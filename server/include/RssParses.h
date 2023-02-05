#ifndef __RSSPARSES_H__
#define __RSSPARSES_H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <tinyxml2.h>
#include <regex>

using namespace tinyxml2;
using std::regex;
using std::string;
using std::vector;

enum MatchType
{
    TITLE,
    LINK,
    DESCRIPTION,
    CONTENT
};

struct RssItem
{
    string title;
    string url;
    string description;
    string content;
};

class RssReader
{
public:
    RssReader(const string &filepath)
    : _filepath(filepath)
    {
        _rss.reserve(1024);
    }
    void parseRss();// 解析
    vector<RssItem>& getrss();
private:
    string _filepath;
    vector<RssItem> _rss;
};

#endif

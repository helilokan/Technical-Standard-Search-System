#ifndef __SPLITTOOL_HPP__
#define __SPLITTOOL_HPP__

#include <string>
#include <vector>

using std::pair;
using std::string;

class SpliteTool
{
public:
    SpliteTool() {}
    virtual ~SpliteTool() {}
    virtual std::vector<pair<string, int>> cut(const std::string &sentence) = 0;
};

#endif
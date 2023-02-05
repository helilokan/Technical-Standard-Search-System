#ifndef __CALCDISTANCE_H__
#define __CALCDISTANCE_H__

#include <string>

using std::string;

class calcDistance
{
public:
    calcDistance(string keyWord)
    : _keyWord(keyWord)
    {}

    // 求取一个字符占据的字节数
    size_t nBytesCode(const char ch);
    // 求取一个字符串的字符长度(根据utf8计算)
    size_t length(const string &str);
    // 计算最小编辑距离，中英文通用
    int editDistance(const string &rhs);

private:
    string _keyWord;
};
#endif


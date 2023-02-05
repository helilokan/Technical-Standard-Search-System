#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include <vector>
#include <string>

using std::vector;
using std::string;

class DirScanner
{
public:
    DirScanner();
    // 对象直接调用traverse
    void operator()(const string &dirname);
    // 获取_files
    vector<string> &files();
    // 递归遍历
    void traverse(const string &dirname);

private:
    vector<string> _files;// 存放每个语料文件的绝对路劲
};

#endif


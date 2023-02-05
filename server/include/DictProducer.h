#ifndef __DICTPRODUCER_HPP__
#define __DICTPRODUCER_HPP__

/* 可以将其当成一个独立程序，该程序输入的是语料，
    输出的是词典文件和词典索引文件 */

#include "CppJieba.h"
#include <vector>
#include <memory>
#include <map>
#include <set>

using std::map;
using std::pair;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;

class Configuration;
class SpliteTool;

class DictProducer
{
public:
    DictProducer(Configuration *conf, SpliteTool *splitetool);
    void buildEnDict(); //创建英语字典
    void buildCnDict(); //创建中文字典
    void createIndex(); //创建索引结点
    void store();       //将词典写入文件
private:
    Configuration *_conf;
    SpliteTool *_spliteTool;
    vector<string> _ch_files;              //中文语料库文件的绝对路径集合
    vector<string> _en_files;           //英文语料库文件的绝对路径集合
    vector<pair<string, int>> _ch_dict;    //中文词典
    vector<pair<string, int>> _en_dict; //英文词典
    map<string, set<int>> _ch_index;    //中文索引
    map<string, set<int>> _en_index; //英文索引
};

#endif

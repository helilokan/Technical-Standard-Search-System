#include "../include/DictProducer.h"
#include "../include/DirScanner.h"
#include "../include/CppJieba.h"
#include "../include/Configuration.h"
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using std::cout;
using std::endl;
using std::make_pair;
using std::transform;
using std::ifstream;
using std::ofstream;
using std::ostringstream;
using std::unordered_map;
using std::unordered_set;

DictProducer::DictProducer(Configuration *conf, SpliteTool *splitetool)
: _spliteTool(splitetool)
, _conf(conf)
{
    DirScanner dir;
    dir(_conf->getConfigMap()["dir_ch"]);
    _ch_files = dir.files();
    dir(_conf->getConfigMap()["dir_en"]);
    _en_files = dir.files();
    
    
}

// 建立英文词典和索引
void DictProducer::buildEnDict()
{
    unordered_map<string, int> en_dict;
    string word;
    unordered_set<string> stop_word = _conf->getStopWordList();
    
    // 遍历每篇英文语料，创建英文词典
    for (auto it : _en_files)
    {
        cout << "en_path: " << it << endl;
        ifstream ifs(it);
        if (!ifs)
        {
            cout << "open file error!" << endl;
        }
        string line;
        // 按行读取
        while (getline(ifs, line))
        {
            string temp;
            // 结束加上等号，防止丢失一行的最后一个词
            for (size_t i = 0; i <= line.size(); i++)
            {
                if (isalpha(line[i]))
                {
                    temp += line[i];
                }
                // 读到非字母认为一个单词读取完成，判断是否为停用词，不是就加入词典
                else
                {
                    transform(temp.begin(), temp.end(), temp.begin(), (int (*)(int))tolower);
                    if (stop_word.find(temp) != stop_word.end())
                    {
                        temp.clear();
                        continue;
                    }
                    else
                    {
                        // 判断是第一次插入还是已经插入了
                        if(en_dict.find(temp) != en_dict.end())
                        {
                            ++en_dict[temp];
                        }
                        else
                        {
                            en_dict[temp] = 1;
                        }
                    }
                    temp.clear();
                }
            }
        }
        ifs.close();
    }

    // 将词典存入
    for (auto &it : en_dict)
    {
        _en_dict.push_back(make_pair(it.first, it.second));
    }

    // 遍历词典建立索引
    int lineNum = 1;
    for (auto it = _en_dict.begin(); it != _en_dict.end(); it++, lineNum++)
    {
        string temp = it->first;
        for (auto & ch : temp)
        {
            string tpch(1, ch);
            _en_index[tpch].insert(lineNum);
        }
    }
}

// 建立中文词典和索引
void DictProducer::buildCnDict()
{
    string content;
    string line;

    // 遍历每篇中文语料文章存储所有的内容
    for (size_t i = 0; i < _ch_files.size(); i++)
    {
        ifstream file(_ch_files[i]);
        while (getline(file, line))
        {
            content += line;
        }
    }

    // 切分出所有的单词，建立中文词典
    _ch_dict = _spliteTool->cut(content);

    // 遍历中文词典建立中文索引，每个中文字占三个字符
    int lineNum = 1;
    for (auto it = _ch_dict.begin(); it != _ch_dict.end(); it++, lineNum++)
    {
        string temp = it->first;
        for (auto it = temp.begin(); it != temp.end(); it += 3)
        {
            // 后面剩的不足一个汉字跳过
            if ((it == temp.end()) || (it + 1) == temp.end() || (it + 2) == temp.end())
            {
                break;
            }
            ostringstream tmp;
            tmp << *it << *(it + 1) << *(it + 2);
            _ch_index[tmp.str()].insert(lineNum);
        }
    }
}

// 将词典和索引写入文件
void DictProducer::store()
{
    //写入中文词典
    ofstream dict_ch(_conf->getConfigMap()["dict_ch"]);
    for (size_t i = 0; i < _ch_dict.size(); i++)
    {
        dict_ch << _ch_dict[i].first << " " << _ch_dict[i].second << "\n";
    }
    dict_ch.close();

    //写入中文词典索引
    ofstream dictIndex_ch(_conf->getConfigMap()["dictIndex_ch"]);
    for (auto it : _ch_index)
    {
        dictIndex_ch << it.first << " ";
        for (auto it2 = it.second.begin(); it2 != it.second.end(); it2++)
        {
            dictIndex_ch << *it2 << " ";
        }
        dictIndex_ch << "\n";
    }
    dictIndex_ch.close();

    //写入英文词典
    ofstream dict_en(_conf->getConfigMap()["dict_en"]);
    for (size_t i = 0; i < _en_dict.size(); i++)
    {
        dict_en << _en_dict[i].first << " " << _en_dict[i].second << "\n";
    }
    dict_en.close();

    //写入中文词典索引
    ofstream dictIndex_en(_conf->getConfigMap()["dictIndex_en"]);
    for (auto it : _en_index)
    {
        dictIndex_en << it.first << " ";
        for (auto it2 = it.second.begin(); it2 != it.second.end(); it2++)
        {
            dictIndex_en << *it2 << " ";
        }
        dictIndex_en << "\n";
    }
    dictIndex_en.close();
}

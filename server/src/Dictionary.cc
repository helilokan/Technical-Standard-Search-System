#include "../include/Dictionary.h"
#include "../include/calcDistance.h"
#include "../include/Configuration.h"
#include <iostream>
#include <fstream>
#include <sstream>


using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::make_pair;

bool operator<(const CandidateResult& lhs, const CandidateResult& rhs)
{
    // 优先级队列默认使用std::less，排序结果是从大到小
    // 编辑距离从小到大排序
    if(lhs._dist != rhs._dist)
    {
        return lhs._dist > rhs._dist;
    }
    // 词频从大到小排序
    else if(lhs._freq != rhs._freq)
    {
        return lhs._freq < rhs._freq;
    }
    // 字典序按照从小到大的顺序排列
    return lhs._word > rhs._word;
}


Dictionary *Dictionary::_pInstance = getInstance();
Dictionary::AutoRelease Dictionary::_ar;
Configuration *Dictionary::_conf = Configuration::getInstance();

Dictionary *Dictionary::getInstance()
{
    if(nullptr == _pInstance)
    {
        _pInstance = new Dictionary();
    }
    return _pInstance;
}


void Dictionary::readDict()
{
    string line;
    string word;
    
    string path = _conf->getConfigMap()["dict_ch"];

    /* cout << path << endl; */
    ifstream dict_ch(path);
    if(!dict_ch.good())
    {
        cout << "ifstream open file error" << endl;
        return;
    }
    while(getline(dict_ch, line))
    {
        istringstream iss(line);
        int freq;
        iss >> word >> freq;
        /* cout << word << endl */
        /*      << freq << endl; */
        _dict.push_back({word, freq});
        /* pair<string, int> tmp = {word, freq}; */
        /* _dict.push_back(tmp); */
    }
    // 记录一下中文关键词的数量，英文的往后存，英文的索引对应的行号需要在此基础上增加
    /* cout << "3333" << endl; */
    _ch_length = _dict.size();
    
    ifstream dict_en(_conf->getConfigMap()["dict_en"]);
    if(!dict_en.good())
    {
        cout << "ifstream open file error" << endl;
        return;
    }
    while(getline(dict_en, line))
    {
        istringstream iss(line);
        int freq;
        iss >> word >> freq;
        _dict.push_back({word, freq});
    }
}

void Dictionary::readIndex()
{
    string line;
    string word;
    
    ifstream dictIndex_ch(_conf->getConfigMap()["dictIndex_ch"]);
    if(!dictIndex_ch.good())
    {
        cout << "ifstream open file error" << endl;
        return;
    }
    while(getline(dictIndex_ch, line))
    {
        istringstream iss(line);
        iss >> word;
        set<int> lines;
        int line_no;
        while(iss >> line_no)
        {
            lines.insert(line_no);
        }
        _index.insert({word, lines});
    }

    
    ifstream dictIndex_en(_conf->getConfigMap()["dictIndex_en"]);
    if(!dictIndex_en.good())
    {
        cout << "ifstream open file error" << endl;
        return;
    }
    while(getline(dictIndex_en, line))
    {
        istringstream iss(line);
        iss >> word;
        set<int> lines;
        int line_no;
        while(iss >> line_no)
        {
            // 英文索引要加上中文部分，以便统一查找
            line_no += _ch_length;
            lines.insert(line_no);
        }
        _index.insert({word, lines});
    }

}

priority_queue<CandidateResult> Dictionary::doQuery(const string& keyWord)
{
    // 先读配置文件
    readDict();
    readIndex();

    // 接收结果
    priority_queue<CandidateResult> result;
    set<CandidateResult> temp;// 用于去重
    // 创建距离计算工具    
    calcDistance calcdis_tool(keyWord);
    // 遍历每个字符
    string ch;
    int cnt1 = 0;
    int cnt2 = 0;
    for(size_t idx = 0, i = 0; i < calcdis_tool.length(keyWord); ++i)
    {
        size_t nBytes = calcdis_tool.nBytesCode(keyWord[idx]);
        ch = keyWord.substr(idx, nBytes);
        idx += (nBytes);
        
        // 未找到的字符跳过
        if(_index.find(ch) == _index.end())
        {
            continue;
        }

        /* cout << "ch = " << ch << endl; */
        for(auto & line_no : _index[ch])
        {
            string word = _dict[line_no - 1].first;
            int freq = _dict[line_no - 1].second;
            int dist = calcdis_tool.editDistance(word);
            // 编辑距离大于等于查询词的长度则排除
            if(dist >= calcdis_tool.length(keyWord))
            {
                continue;
            }

            CandidateResult cand_word(word, freq, dist);
            // 去重
            auto it = temp.insert(cand_word);
            if(it.second)
            {
                result.push(cand_word);
                ++cnt1;
            }
            ++cnt2;
        }
    }
    cout << "共处理单词：" << cnt2 << endl
         << "剩余单词：" << cnt1 << endl;

    return result;
}


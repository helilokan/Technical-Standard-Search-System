#ifndef __CPPJIEBA_HPP__
#define __CPPJIEBA_HPP__

#include "SplitTool.h"
#include "../include/simhash/cppjieba/Jieba.hpp"

const char *const DICT_PATH = "../include/simhash/dict/jieba.dict.utf8";      //最大概率法(MPSegment: Max Probability)分词所使用的词典路径
const char *const HMM_PATH =  "../include/simhash/dict/hmm_model.utf8";        //隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
const char *const IDF_PATH = "../include/simhash/dict/idf.utf8";              // IDF路径
const char *const STOP_WORD_PATH = "../include/simhash/dict/stop_words.utf8"; //停用词路径

using std::pair;
using std::string;

class Configuration;

class CppJieba
: public SpliteTool
{
public:
    CppJieba(Configuration *_conf);
    ~CppJieba();
    virtual std::vector<pair<string, int>> cut(const std::string &sentence);

private:
    cppjieba::Jieba _jieba;
    Configuration *_conf;
};

#endif

#include "../include/mySimhasher.h"
#include "../include/Configuration.h"

const char *const DICT_PATH = "../include/simhash/dict/jieba.dict.utf8";      //最大概率法(MPSegment: Max Probability)分词所使用的词典路径
const char *const HMM_PATH =  "../include/simhash/dict/hmm_model.utf8";        //隐式马尔科夫模型(HMMSegment: Hidden Markov Model)分词所使用的词典路径
const char *const IDF_PATH = "../include/simhash/dict/idf.utf8";              // IDF路径
const char *const STOP_WORD_PATH = "../include/simhash/dict/stop_words.utf8"; //停用词路径


const int mySimhasher::TOPK_NUMBER = 20;


mySimhasher::mySimhasher(Configuration *conf)
: _conf(conf)
, _simhasher(DICT_PATH, HMM_PATH, IDF_PATH, STOP_WORD_PATH)
, _jieba(DICT_PATH, HMM_PATH, "")
{
    _stopWordList = _conf->getStopWordList();
    cout << "mySimhasher()" << endl;
}

mySimhasher::~mySimhasher()
{
    cout << "~mySimhasher()" << endl;
}

uint64_t mySimhasher::calcSimhash(const string &text)
{
    uint64_t tmp;
    _simhasher.make(text, TOPK_NUMBER, tmp);
    return tmp;
}

unordered_map<string, int> mySimhasher::cut(const string &text)
{
    // 存储切分后的词
    vector<string> allwords;
    _jieba.Cut(text, allwords, true);
    // 存储去除停用词后的单词
    vector<string> Erase_StopWord;
    for (auto &it : allwords)
    {
        if (_stopWordList.find(it) != _stopWordList.end())
        {
            continue;
        }
        else
        {
            Erase_StopWord.push_back(it);
        }
    }
    // 存储最终的词和词频
    unordered_map<string, int> word_query;
    // 计算词频
    for (auto &it : Erase_StopWord)
    {
        unsigned char temp = it[0];
        // 如果是英文改为小写
        if(isalpha(temp))
        {
            transform(it.begin(), it.end(), it.begin(), (int(*)(int))tolower);
        }
        // 其他字符全部排除
        else if(temp < 128)
        {
            continue;
        }
        word_query[it]++;    
    }

    return word_query;
}

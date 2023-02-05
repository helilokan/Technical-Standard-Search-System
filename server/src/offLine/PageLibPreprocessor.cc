#include "../../include/PageLibPreprocessor.h"
#include "../../include/Configuration.h"
#include <iostream>
#include <fstream>
#include <math.h>

using std::cout;
using std::endl;
using std::ofstream;

PageLibPreprocessor::PageLibPreprocessor(mySimhasher &mysh)
: _mysh(mysh)
{
    cout << "PageLibPreprocessor()" << endl;
    /* doProcess(); */
    /* buildInvertIndexTable(); */
    /* storeOnDisk(); */
}

// 读取网页文件，存储网页库，并去重，记录偏移信息
void PageLibPreprocessor::doProcess()
{
    // 1. 读取配置文件获取xml文件所在目录，读取目录下的所有文件
    Configuration * pconf = Configuration::getInstance();
    DirScanner dir;
    string dirpath = pconf->getConfigMap()["xml"];
    dir(dirpath);
    string doc;
    int docId = 1;
    int offset = 0;
    int totalpage = 0;
    int emptypage = 0;
    // 2. 对目录下的每一篇文章进行处理构建网页库和网页偏移库
    for(auto & path : dir.files())
    {
        doc = path.substr(dirpath.size() + 1);
        cout << "doc = " << doc << endl; 
        
        RssReader rss(path);
        rss.parseRss();
        for(auto & elem : rss.getrss())
        {
            // 记录总文章数
            ++totalpage;
            
            size_t l = elem.content.size() + elem.description.size();
            // 跳过内容为空的文章
            if(0 == l)
            {
                ++emptypage;
                continue;
            }

            WebPage webpage(doc, docId, elem, _mysh);
            // 插入时set会自动去重
            // 在WebPage类的重载 operator<() 中定义用 simhash 判断相同的文章返回 false
            auto ret = _pageLib.insert(webpage);
            // 插入成功，则存储偏移信息
            if(ret.second)
            {
                // 插入文章id，文章内容起始位置，文章内容长度
                _offsetLib.insert({docId, {offset, l}});
                /* cout << "success" << endl; */
                // 55是网页库中的分类信息如"<doc>"
                // 加上docid长度，url长度，title长度即为文章内容在网页库中的起始位置
                string t = std::to_string(docId);
                offset += 55 + t.size() + elem.url.size() + elem.title.size();
                // 加上文章内容部分即后面部分的长度，为下一次偏移做准备
                offset += l + 16 + 1;// 算上最后一个回车
                // 插入成功的文中记录ID
                ++docId;
            }
        }
    }
    cout << "总文章数: " << totalpage << endl
         << "去重后文章数: " << docId - 1 << endl
         << "空文章数: " << emptypage << endl
         << "重复文章数: " << totalpage - docId + 1 - emptypage << endl;
    /* cout << "pageLib.size() = " << _pageLib.size() << endl; */

}

// 创建倒排索引表
void PageLibPreprocessor::buildInvertIndexTable()
{
    // 用于存储每个单词在所有文章中的总词频
    map<string, int> total;
    for(auto &page : _pageLib)
    {
        for(auto &elem : page._wordsMap)
        {
            string word = elem.first;
            auto cnt = total.count(word);
            if(cnt == 0)
            {
                total[word] = elem.second;
            }
            else
            {
                total[word] += elem.second;
            }
        }
    }

    // 计算权重
    double N = _pageLib.size();
    for(auto &page : _pageLib)
    {
        double sum = 0;
        map<string, double> w;
        // 1. 计算一篇文章中的每个词的权重及所有词的权重之和
        for(auto &elem :page._wordsMap)
        {
            string word = elem.first;
            double TF = elem.second;
            double DF = total[word];
            double IDF = log2(N / (DF + 1));
            w[word] = TF * IDF;
            sum += 2 * TF * IDF;
        }
        // 2. 将这篇文章中的每个词归一化处理后插入倒排索引表
        for(auto &elem : w)
        {
            double final_w = elem.second/sqrt(sum);
            _invertIndexTable[elem.first].push_back({page._docId, final_w});
        }
    }

}

// 将网页库和倒排索引库写回到磁盘
void PageLibPreprocessor::storeOnDisk()
{
    Configuration * pconf = Configuration::getInstance();
    string ripepage = pconf->getConfigMap()["ripepage"];
    ofstream ofs1(ripepage);
    
    for(auto & elem : _pageLib)
    {
        ofs1 << "<doc><docid>" << elem._docId
             << "</docid><url>" << elem._docUrl
             << "</url><title>" << elem._docTitle
             << "</title><content>" << elem._docContent
             << "</content></doc>\n";
    }

    string offset = pconf->getConfigMap()["offset"];
    ofstream ofs2(offset);
    
    for(auto & elem : _offsetLib)
    {
        ofs2 << elem.first << " " 
             << elem.second.first << " " 
             << elem.second.second << "\n";
    }


    string ivertIndex = pconf->getConfigMap()["ivertIndex"];
    ofstream ofs3(ivertIndex);

    for(auto & elem : _invertIndexTable)
    {
        ofs3 << elem.first << " ";
        for(auto & p : elem.second)
        {
            ofs3 << " " << p.first << " " << p.second << " ";
        }
        ofs3 << "\n";
    }
    ofs1.close();
    ofs2.close();
    ofs3.close();
}



#include "../include/WebPageQuery.h"

WebPageQuery::WebPageQuery(Configuration *conf)
: _smhs(conf)
, _conf(conf)
{ 
    loadLibrary();
}


void WebPageQuery::loadLibrary()
{
    //创建偏移库容器
    ifstream offset(_conf->getConfigMap()["offset"]);
    if (!offset.good())
    {
        perror("open newoffset.dat");
        return;
    }
    string IdAndOffset;
    int docId = 0;
    while (offset.good())
    {
        for (int i = 0; i < 3; i++)
        {
            offset >> IdAndOffset;
            if (i == 0)
            {
                docId = stoi(IdAndOffset);
            }
            if (i == 1)
            {
                _offsetLib[docId].first = stoi(IdAndOffset);
            }
            if (i == 2)
            {
                _offsetLib[docId].second = stoi(IdAndOffset);
            }
        }
    }
    offset.close();

    //创建倒排索引库容器
    ifstream invert(_conf->getConfigMap()["ivertIndex"]);
    if (!invert.good())
    {
        perror("open invertIndex.dat");
        return;
    }
    string line;
    while (getline(invert, line))
    {
        istringstream iss(line);
        string key;
        docId = 0;
        double weight;
        iss >> key;
        while (iss >> docId)
        {
            iss >> weight;
            _invertIndexTable[key].insert(make_pair(docId, weight));
        }
    }
    invert.close();
}

std::string ch2bit(char ch);

std::wstring s2ws(const std::string &str);

std::string ws2s(const std::wstring &wstr);

void WebPageQuery::bulitBaseVector()
{
    double sum = 0;

    auto it = _freq.begin();
    while (it != _freq.end())
    {
        int TF = it->second;
        int DF = 0;
        if (_invertIndexTable.find(it->first) != _invertIndexTable.end())
        {
            DF = _invertIndexTable[it->first].size();
        }
        int N = _offsetLib.size();
        double IDF = log(N / (DF + 1) / log(2));
        double w = TF * IDF; //权重
        _BaseVector[it->first] = w;

        sum += w * w; //权重平方和
        ++it;
    }

    for (auto &f : _BaseVector)
    {
        f.second /= sum;
    }
}

void WebPageQuery::findKeyPageWeb()
{
    for(auto & e : _invertIndexTable)
        {
            /* cout << e.first << endl; */
            if("hello" == e.first)
            {
                cout << e.first << endl;
            }       
        }
    for (auto &it : _freq)
    {
        /* cout << "_SplittedString :" <<it<<endl; */
        vector<int> tmp;
        auto ret = _invertIndexTable.find(it.first);
        
        /* cout << "before find" << endl; */
        /* cout << it.first << endl; */
        
        if (ret != _invertIndexTable.end())
        {
            /* cout << "find it " << endl; */
            _keyWord.insert(it.first);
            
            for (auto &bt : _invertIndexTable[it.first])
            {
                tmp.push_back(bt.first);
            }
        }
        if (!tmp.empty())
        {
            _pagesIdQueryed.push_back(tmp);
        }
    }
}

void WebPageQuery::contAllKeyWordPageID()
{
    if (!_pagesIdQueryed.empty())
    {
        set<int> result;

        set<int> tmp(_pagesIdQueryed.front().begin(),
                     _pagesIdQueryed.front().end());
        for (auto &f : _pagesIdQueryed)
        {
            set<int> fs(f.begin(), f.end());
            std::set_intersection(fs.begin(), fs.end(), tmp.begin(), tmp.end(),
                                  std::insert_iterator<set<int>>(result, result.begin()));
            tmp = result;
            result.clear();
        }
        result = tmp;
        for (auto f : result)
        {
            _AllKeyWordPageID.push_back(f);
        }
    }
}

void WebPageQuery::coutCosWeight()
{
    size_t cnt = 0;
    for (auto &reserveSize : _AllKeyWordPageID)
    {
        _CosWeight[reserveSize] = vector<double>(_BaseVector.size());
    }
    for (auto &f : _BaseVector)
    {
        //去倒排索引里面找docid
        auto &docidSet = _invertIndexTable[f.first];
        for (auto &pairKv : docidSet)
        {
            for (size_t idx = 0; idx < _AllKeyWordPageID.size(); ++idx)
            {
                int docid = _AllKeyWordPageID[idx];
                if (docid == pairKv.first)
                {
                    _CosWeight[docid][cnt] = pairKv.second;
                }
            }
        }
        ++cnt;
    }
}

void WebPageQuery::CosPageWebRank()
{
    vector<double> Xvector;
    for (auto &f : _BaseVector)
    {
        Xvector.push_back(f.second);
    }

    for (auto &f : _CosWeight)
    {
        int docid = f.first;
        vector<double> Yvector = f.second;

        double XY = 0;
        double XX = 0;
        double YY = 0;
        for (size_t idx = 0; idx != Xvector.size(); ++idx)
        {
            XY += Xvector[idx] * Yvector[idx];

            XX += Xvector[idx] * Xvector[idx];
            YY += Yvector[idx] * Yvector[idx];
        }

        double Cos = XY / (sqrt(XX) * sqrt(YY));

        _CosPageWebRank.push_back(make_pair(docid, Cos));
    }
    std::sort(_CosPageWebRank.begin(), _CosPageWebRank.end(),
              [&](const pair<int, double> &a, const pair<int, double> &b)
              { return a.second > b.second; });
}

void WebPageQuery::makeAbs()
{

    size_t AbsNum = _CosPageWebRank.size();
    size_t articleNum = (AbsNum > 5) ? 5 : AbsNum;
    
    /* cout << "articleNum = " << articleNum << endl; */
    for (size_t idx = 0; idx < articleNum; ++idx)
    {
        int docid = _CosPageWebRank[idx].first;
        /* cout << "docid = " << docid << endl; */
        int offset = _offsetLib[docid].first;
        /* cout << "offset = " << offset << endl; */
        ifstream ifsPage(_conf->getConfigMap()["ripepage"]);
        if (!ifsPage.good())
        {
            cerr << "newpage.lib open erron" << endl;
        }

        ifsPage.seekg(offset, std::ios_base::beg);

        string PageXML;
        getline(ifsPage, PageXML);


        /* cout << "PageXML: " << endl */
        /*      << PageXML << endl; */
        //以上程序都正常
        XMLDocument xml;
        if (xml.Parse(PageXML.c_str()))
        {
            cout << "docid : " << docid << endl;
            cout << xml.ErrorIDToName(xml.ErrorID()) << endl;
            continue;
        }
        
        XMLElement *root = xml.RootElement();
        string Docid = root->FirstChildElement("docid")->GetText();
        /* docid = atoi(root->FirstChildElement("docid")->GetText()); */
        docid = atoi(Docid.c_str());
        /* cout << "docid = " << docid << endl; */
        string URL = root->FirstChildElement("url")->GetText();
        string Title = root->FirstChildElement("title")->GetText();
        string Content = root->FirstChildElement("content")->GetText();

        /* cout << "URL = " << URL << endl; */
        /* cout << "Title = " << Title << endl; */
        /* cout << "Content = " << Content << endl; */


        vector<string> topThreeKey;

        auto it = _BaseVector.begin();
        int num = 0;
        while (num < 3 && it != _BaseVector.end())
        {
            topThreeKey.push_back(it->first);
            ++num;
            ++it;
        }

        vector<int> keyWordBegEnd;

        auto wContent = s2ws(Content);

        for (size_t idx = 0; idx < topThreeKey.size(); ++idx)
        {
            size_t N = wContent.find(s2ws(topThreeKey[idx]));
            keyWordBegEnd.push_back(N);
        }

        /* cout << "keyWordBegEnd = " << keyWordBegEnd << endl; */
        // 删除相互重叠的摘要
        if (keyWordBegEnd.size() > 1)
        {
            sort(keyWordBegEnd.begin(), keyWordBegEnd.end());

            int pre_value = keyWordBegEnd.front();
            keyWordBegEnd.erase(
                remove_if(keyWordBegEnd.begin() + 1,
                          keyWordBegEnd.end(),
                          [&](int &pos) -> bool
                          {
                              if (pos - pre_value < 40)
                              {
                                  return true;
                              }
                              else
                              {
                                  pre_value = pos;
                                  return false;
                              }
                          }),
                keyWordBegEnd.end());
        }
        // 取前40字和后40字
        std::wstring wAbsString;
        for (auto &f : keyWordBegEnd)
        {
            auto abs_start = (f - 40 < 0) ? 0 : f - 40;
            auto abs_size = (f + 40 >= int(wContent.size())) ? int(wContent.size()) - f + 40 : 80;
            wAbsString += wContent.substr(abs_start, abs_size);
            wAbsString += L"......";
        }

        string AbsString = ws2s(wAbsString);

        string WebPageString = "[标题]  " + Title + "\n";
        WebPageString += "[网址]  " + URL + "\n";
        WebPageString += "[摘要]  " + AbsString + "\n";

        /* cout << "docid : " << docid << endl; */
        /* cout << WebPageString << endl; */
        /* cout << endl; */
        _Abs.push_back(WebPageString);
    }
    cout << "WebPageMakeDone" << endl;
}

size_t WebPageQuery::nBytes(char ch)
{
    size_t res = 1;
    int N = 6;
    if (ch & (1 << 7))
    {
        while ((ch & (1 << N)) != 0)
        {
            ++res;
            --N;
        }
    }
    return res;
}

vector<string> WebPageQuery::doQuery(const string &QueryString)
{
    /*  doSplit(QueryString);
     bulitFreqMap(); */
    cout << "doQuery" << endl;
    cout << "QueryString = " << QueryString << endl;
    _freq = _smhs.cut(QueryString);
    /* for(auto & elem : _freq) */
    /* { */
    /*     cout << elem.first << " "; */
    /* } */
    cout << endl;
    /* cout << "bulitBaseVector" << endl; */
    bulitBaseVector();
    findKeyPageWeb();
    contAllKeyWordPageID();
    coutCosWeight();
    CosPageWebRank();
    makeAbs();
    return _Abs;
}

void WebPageQuery::clear()
{
    _pagesIdQueryed.clear();
    _AllKeyWordPageID.clear();
    _CosWeight.clear();
    _CosPageWebRank.clear();
    _freq.clear();    //分词的词频
    _keyWord.clear(); //在倒排索引表中出现的词
    _BaseVector.clear();
    _QueryKeyWord.clear();
    _Abs.clear();
}

std::wstring s2ws(const std::string &str)
{
    // 这步不成功去命令行输入locale检查有无中文环境
    /* std::locale mylocale(u8"zh_CN.UTF-8"); */
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    // 预先定义出错时返回的语句
    // std::wstring_convert<convert_typeX, wchar_t> converterX{"error convert"};
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    try
    {
        return converterX.from_bytes(str);
    }
    catch (std::exception &e)
    {
        cerr << "error : " << e.what() << endl;
        cout << "converterX.converted() : " << converterX.converted() << endl;
        size_t error_pos = converterX.converted() - 10 > 0 ? converterX.converted() - 10 : 0;
        // 查看出错处前5个字到文件结尾的ASCII码和二进制码
        for (size_t pos = converterX.converted() - 5; pos < str.size(); ++pos)
        {
            cout << pos << " : " << str.at(pos) << "  " << static_cast<int>(str.at(pos)) << "\t" << ch2bit(str.at(pos)) << endl;
        }
        cout << "error text : " << str.substr(error_pos) << endl;

        cout << endl;
        // 返回可成功转化的字符串
        return converterX.from_bytes(str.c_str(), str.c_str() + converterX.converted());
    }
}

std::string ws2s(const std::wstring &wstr)
{
    /* std::locale mylocale(u8"zh_CN.UTF-8"); */
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    // std::wstring_convert<convert_typeX, wchar_t> converterX{"error convert"};
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    try
    {
        return converterX.to_bytes(wstr);
    }
    catch (std::exception &e)
    {
        cout << "error : " << e.what() << endl;
        cout << "converterX.converted() : " << converterX.converted() << endl;
        size_t error_pos = converterX.converted() - 10 > 0 ? converterX.converted() - 10 : 0;
        cout << "error text : " << wstr.substr(error_pos).c_str() << endl;
        return converterX.to_bytes(wstr.c_str(), wstr.c_str() + converterX.converted());
    }
}

std::string ch2bit(char ch)
{
    std::string ret;

    for (int N = 7; N >= 0; --N)
    {
        ret += (ch & (1 << N)) ? "1" : "0";
    }

    return ret;
}

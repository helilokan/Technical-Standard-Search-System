#include "../include/KeyRecommander.h"
#include <iostream>

using std::cout;
using std::endl;

KeyRecommander::KeyRecommander(const string &sought)
: _sought(sought)
{

}

string KeyRecommander::doQuery()
{
    Dictionary *dic = Dictionary::getInstance();
    _prique = dic->doQuery(_sought);
    
    string result;
    int idx = 0;
    while(!_prique.empty() && idx < 20)
    {
        result += _prique.top()._word + " ";
        _prique.pop();
        ++idx;
    }
    return result;
}


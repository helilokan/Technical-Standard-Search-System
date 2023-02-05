#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include "Dictionary.h"

class KeyRecommander
{
public:
    KeyRecommander(const string &sought);
    string doQuery();

private:
    string _sought;
    priority_queue<CandidateResult> _prique;
};
#endif


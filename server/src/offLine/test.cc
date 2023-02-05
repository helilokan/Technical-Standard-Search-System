#include "../include/PageLibPreprocessor.h"
#include "../include/mySimhasher.h"
#include "../include/Configuration.h"
#include "../include/DictProducer.h"

#include <iostream>

using std::cout;
using std::endl;

int test0()
{
    Configuration *conf = Configuration::getInstance();
    
    cout << "begin pagelib" << endl;
    mySimhasher mysh(conf);
    PageLibPreprocessor pagelib(mysh);
    pagelib.doProcess();
    pagelib.buildInvertIndexTable();
    pagelib.storeOnDisk();
    cout << "end pagelib" << endl;

    cout << "begin dict" << endl;
    CppJieba jieba(conf);
    SpliteTool *split = &jieba;
    DictProducer dic(conf, split);
    dic.buildCnDict();
    dic.buildEnDict();
    dic.store();
    cout << "end dic" << endl;
}

int main()
{
    test0();
    return 0;
}


#include "../include/mylogger.h"


void Mylogger::init_root()
{
    PatternLayout * ptnLayout1 = new PatternLayout();
    ptnLayout1->setConversionPattern("%d [%c] [%p] %m%n");
    PatternLayout * ptnLayout2 = new PatternLayout();
    ptnLayout2->setConversionPattern("%d [%c] [%p] %m%n");

    OstreamAppender * posAp = new OstreamAppender("console", &cout);
    posAp->setLayout(ptnLayout1);

    auto rollingFileApp = new RollingFileAppender("rollingFileApp", "../log/rollingwd.log", 5000, 9);
    rollingFileApp->setLayout(ptnLayout2);

    _root.setPriority(Priority::ERROR);
    /* _root.setAppender(posAp); */
    _root.addAppender(rollingFileApp);
}

Mylogger * Mylogger::getInstance()
{
    pthread_once(&_once, init);
    return _pInstance;
}

void Mylogger::init()
{
    _pInstance = new Mylogger();
    init_root();
    atexit(destroy);
}

void Mylogger::destroy()
{
    if(_pInstance)
    {
        delete _pInstance;
        _pInstance = nullptr;
    }
}


pthread_once_t Mylogger::_once = PTHREAD_ONCE_INIT;

Mylogger * Mylogger::_pInstance = nullptr;

Category & Mylogger::_root = Category::getRoot();

Category & Mylogger::_module1 = _root.getInstance("test");




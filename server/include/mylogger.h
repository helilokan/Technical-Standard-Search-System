#ifndef __MYLOGGER_H__
#define __MYLOGGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
using std::string;
using std::cout;
using std::endl;
using namespace log4cpp;


class Mylogger
{
public:

    static void init_root();
    
    static Mylogger * getInstance();
    
    static void init();
    
    static void destroy();
    template<class ...Args>
    void emerg(const char *msg, Args ...args)
    {
        _module1.emerg(msg, args...);
    }
    template<class ...Args>
    void alert(const char *msg, Args ...args)
    {
        _module1.alert(msg, args...);
    }
    template<class ...Args>
    void crit(const char *msg, Args ...args)
    {
        _module1.crit(msg, args...);
    }
    template<class ...Args>
    void error(const char *msg, Args ...args)
    {
        _module1.error(msg, args...);
    }
    template<class ...Args>
    void warn(const char *msg, Args ...args)
    {
        _module1.warn(msg, args...);
    }
    template<class ...Args>
    void notice(const char *msg, Args ...args)
    {
        _module1.notice(msg, args...);
    }
    template<class ...Args>
    void info(const char *msg, Args ...args)
    {
        _module1.info(msg, args...);
    }
    template<class ...Args>
    void debug(char *msg, Args ...args)
    {
        _module1.debug(msg, args...);
    }
    
private:
    Mylogger(){};
    ~Mylogger(){};
    static pthread_once_t _once;
    static Category & _root;
    static Category & _module1;
    static Mylogger * _pInstance;
};



#endif

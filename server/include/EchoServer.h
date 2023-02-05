#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "CacheManager.h"
#include "ThreadPool.h"
#include "TcpServer.h"
#include "KeyRecommander.h"
#include "WebPageQuery.h"
#include "mylogger.h"
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

#define WRITE(buf, a) do{sprintf(buf, "[%s@%s,%d] %s", __func__, __FILE__, __LINE__, a);}while(0)
#define logEmerg(a, ...)    do{char buf[4096] = {0};WRITE(buf, a);log->emerg(buf, __VA_ARGS__);}while(0)
#define logAlert(a, ...)    do{char buf[1024] = {0};WRITE(buf, a);log->alert(buf, __VA_ARGS__);}while(0)
#define logCrit(a, ...)     do{char buf[1024] = {0};WRITE(buf, a);log->crit(buf, __VA_ARGS__);}while(0)
#define logError(a, ...)    do{char buf[1024] = {0};WRITE(buf, a);log->error(buf, __VA_ARGS__);}while(0)
#define logWarn(a, ...)     do{char buf[1024] = {0};WRITE(buf, a);log->warn(buf, __VA_ARGS__);}while(0)
#define logNotice(a, ...)   do{char buf[1024] = {0};WRITE(buf, a);log->notice(buf, __VA_ARGS__);}while(0)
#define logInfo(a, ...)     do{char buf[1024] = {0};WRITE(buf, a);log->info(buf, __VA_ARGS__);}while(0)
#define logDebug(a, ...)    do{char buf[1024] = {0};WRITE(buf, a);log->debug(buf, __VA_ARGS__);}while(0)


class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con, CacheManager *cach)
        : _cach(cach)
          , _msg(msg)
          , _con(con)
    {

    }

    void process(unsigned long addr)
    {
        // 处理传输来的信息，解析Json

        istringstream iss(_msg);
        int check;

        string msg;
        string tmp;
        iss >> check ;
        while(iss >> tmp)
        {
            msg += tmp + " ";
        }
        msg = msg.substr(0 , msg.size() - 1);
        if(1 == check)
        {
            // 1. 关键词推荐
            // 先查当前线程对应的cache
            string result = _cach->findInCache(addr, "1" + msg);

            // 如果cache中没有，则查词典
            if("err" == result)
            {
                cout << "Not in cache" << endl;

                KeyRecommander kr(msg);
                string key = "1" + msg;
                _msg = kr.doQuery();


                vector<string> val;
                istringstream viss(_msg);
                while(viss >> tmp)
                {
                    val.push_back(tmp);
                }
                // 将新的查询结果存入cache
                _cach->addInCache(addr, key, val);
            }
            else
            {
                _msg = result;
            }
        }
        else if(2 == check)
        {
            // 2. 网页查询
            // 先查当前线程对应的cache
            cout << "msg = " << msg << endl;
            istringstream msg_iss(msg);
            string key = "2";
            while(msg_iss >> tmp)
            {
                key += tmp;
            }
            string result = _cach->findInCache(addr, key);
            // 如果cache中没有，则查词典
            if("err" == result)
            {
                cout << "Not in cache" << endl;
                Configuration *conf = Configuration::getInstance();
                WebPageQuery qr(conf);

                vector<string> val = qr.doQuery(msg);
                if(val.empty())
                {
                    cout << "no such doc" << endl;
                    _msg = "no such doc\n";
                }
                else
                {
                    string r;
                    for(auto & elem : val)
                    {
                        r += elem;
                        r += " ";
                    }
                    _msg = r;

                    istringstream val_iss(r);
                    val.clear();
                    while(val_iss >> tmp)
                    {
                        if(tmp == "\n")
                        {
                            continue;
                        }
                        val.push_back(tmp);
                    }

                    // 将新的查询结果存入cache
                    _cach->addInCache(addr, key, val);

                }
            }
            else
            {
                _msg = result;
            }

        }
        else
        {
            _msg = "err";
        }
        // 将查询结果给主线程发回给客户端
        _con->sendInLoop(_msg);
    }

private:
    CacheManager *_cach;
    string _msg;
    TcpConnectionPtr _con;
};


class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize, const string &ip, unsigned short port, CacheManager *cach)
        : _cach(cach)
          , _pool(threadNum, queSize, cach)
          , _server(ip, port, cach)
    {

    }

    void start()
    {
        _pool.start();
        using namespace std::placeholders;
        _server.setAllCallback(bind(&EchoServer::onConnection, this, _1)
                               , bind(&EchoServer::onMessage, this, _1)
                               , bind(&EchoServer::onClose, this, _1));
        _server.start();// 
    }

    void stop()
    {
        _pool.stop();
        _server.stop();
    }


    void onConnection(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has connected!" << endl;
        Mylogger *log = Mylogger::getInstance();
        logEmerg("%s has connected!\n", con->toString().c_str()); 
    }

    void onMessage(const TcpConnectionPtr &con)
    {
        string msg = con->receive();
        cout << "recv msg " << msg << endl;

        Mylogger *log = Mylogger::getInstance();
        logEmerg("%s recv msg %s\n", con->toString().c_str(), msg); 
        MyTask task(msg, con, _cach);
        using namespace std::placeholders;
        _pool.addTask(std::bind(&MyTask::process, task, _1));
    }

    void onClose(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has closed!" << endl;
        Mylogger *log = Mylogger::getInstance();
        logEmerg("%s has closed!\n", con->toString().c_str()); 
    }
private:
    CacheManager *_cach;
    ThreadPool _pool;
    TcpServer _server;
};


#endif

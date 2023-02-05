#ifndef __ECHOSERVER_H__
#define __ECHOSERVER_H__

#include "ThreadPool.h"
#include "TcpServer.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    {

    }

    void process()
    {
        // 在process函数中进行真正的业务逻辑处理
        // ...
        // ...
        // 现在计算线程处理完业务逻辑之后需要将处理后的数据发送给IO线程(EventLoop/Reactor)
        // 通过eventfd进行线程间通信
        _con->sendInLoop(_msg);
    }

private:
    string _msg;
    TcpConnectionPtr _con;
};


class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize, const string &ip, unsigned short port)
    : _pool(threadNum, queSize)
    , _server(ip, port)
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
    }
    
    void onMessage(const TcpConnectionPtr &con)
    {
        // 回显
        string msg = con->receive();
        cout << "recv msg   " << msg << endl;
        // 接收与发送之间，消息msg是没有做任何处理的
        // 处理msg的业务逻辑全部在此处实现的话
        // 将msg这些信息打个包交给MyTask，然后将MyTask注册给线程池
        // 让线程池去处理具体的业务逻辑
        // 此时业务逻辑的处理就不在EventLoop线程中
    
        // 建立计算任务，交给线程池处理
        MyTask task(msg, con);
        /* gThreadPool->addTask(std::bind(&MyTask::process, &task)); */
        _pool.addTask(std::bind(&MyTask::process, task));
        
        con->send(msg);// 回写一下
        cout << "1. got msg" << endl;
    
    }
    
    void onClose(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has closed!" << endl;
    }
private:
    ThreadPool _pool;
    TcpServer _server;
};


#endif

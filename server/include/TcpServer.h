#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

class CacheManager;

class TcpServer
{
public:
    TcpServer(const string &ip, unsigned short port, CacheManager *cach)
    : _acceptor(ip, port)
    , _loop(_acceptor, cach)
    {

    }

    void start()
    {
        _acceptor.ready();
        _loop.loop();
    }

    void stop()
    {
        _loop.unloop();
    }

    void setAllCallback(TcpConnectionCallback &&onConnection
                        , TcpConnectionCallback &&onMessage
                        , TcpConnectionCallback &&onClose)
    {
        _loop.setConnectionCallback(std::move(onConnection));
        _loop.setMessageCallback(std::move(onMessage));
        _loop.setCloseCallback(std::move(onClose));
    }

private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif

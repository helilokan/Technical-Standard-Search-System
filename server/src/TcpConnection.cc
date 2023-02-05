#include "../include/TcpConnection.h"
#include "../include/EventLoop.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd, EventLoop *loop)
: _loop(loop)
, _sock(fd)
, _sockIO(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
{

}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::send(const string &msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const string &msg)
{
    if(_loop)
    {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}

string TcpConnection::receive()
{
    char buff[65535] = {0};
    _sockIO.readLine(buff, sizeof(buff));

    return string(buff);
}

string TcpConnection::toString()
{
    ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "---->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();

    return oss.str();
}

bool TcpConnection::isClosed() const
{
    char buf[10] = {};
    // 检查一下数据，不取出
    int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);

    return (ret == 0);
}

InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getsockname");
    }
    
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
    if(-1 == ret)
    {
        perror("getpeername");
    }

    return InetAddress(addr);
}

// 注册回调函数
void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb)
{
    _onConnectionCb = std::move(cb);// 这里用右值不用右值都可以
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb)
{
    _onMessageCb = std::move(cb);
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb)
{
    _onCloseCb = std::move(cb);
}


// 执行回调函数
void TcpConnection::handleConnectionCallback()
{
    if(_onConnectionCb)
    {
        /* using TcpConnectionPtr = shared_ptr<TcpConnection>; */
        /* using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>; */
        // function<void(const shared_ptr<TcpConnection> &) _onConnectionCb
        _onConnectionCb(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessageCb)
    {
        _onMessageCb(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback()
{
    if(_onMessageCb)
    {
        _onCloseCb(shared_from_this());
    }
}

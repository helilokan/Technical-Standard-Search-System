#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"
#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;

class EventLoop;// 前向声明

// 在返回this指针的时候如果用shared_ptr智能指针接管
// 则会有两个指针托管同一堆空间，两个指针释放时，double free问题
// 利用enable_shared_from_this类中的成员函数 shared_from_this()返回托管this的只能指针
// 不会重复复制
class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
{
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
public:
    TcpConnection(int fd, EventLoop *loop);
    ~TcpConnection();
    void send(const string &msg);
    void sendInLoop(const string &msg);
    string receive();
    string toString();

    bool isClosed() const;

    // 注册三个事件的回调函数
    void setConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);

    // 三个事件回调函数的执行
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    EventLoop *_loop;// 用于与IO线程通信
    Socket _sock;
    SocketIO _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

};
#endif


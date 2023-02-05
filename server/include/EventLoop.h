#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "MutexLock.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "CacheManager.h"
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
using Functor = function<void()>;

class CacheManager;

class EventLoop
{
public:
    EventLoop(Acceptor &acceptor, CacheManager *cach);
    ~EventLoop();
    void loop();
    void unloop();

    void runInLoop(Functor &&cb);

        void setConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

private:
    void wakeup();
    void waitEpollFd();// epoll_wait
    void handleNewConnection();// 有新连接
    void handleRead();// 子线程发来消息
    void doPengingFunctors();// IO线程的任务
    void handleMessage(int fd);// 已经建立的连接中有消息传来
    void addEpollReadFd(int fd);// 将新的连接加入监听
    void delEpollReadFd(int fd);// 将连接从监听中去除
    int createEpollFd();// 创建监听
    int createEventFd();// 建立线程间通信

private:
    int _epfd;// epoll监听队列红黑树的根节点
    int _evtfd;// 用于线程间通信
    Acceptor &_acceptor;
    CacheManager *_cach;
    bool _isLooping;
    vector<struct epoll_event> _evtList;// epoll的就绪队列
    map<int, TcpConnectionPtr> _conns;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;

    vector<Functor> _pengingsCb;// 需要处理的IO事件队列
    MutexLock _mutex;// 调用PengingsCb是多线程并发的，需要上锁
};
#endif


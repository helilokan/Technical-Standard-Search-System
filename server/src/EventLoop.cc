#include "../include/EventLoop.h"
#include "../include/CacheManager.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor &acceptor, CacheManager *cach)// _mutex会默认构造
: _epfd(createEpollFd())
, _evtfd(createEventFd())
, _cach(cach)
, _acceptor(acceptor)
, _isLooping(false)
, _evtList(1024)
{
    addEpollReadFd(acceptor.fd());// 把listenfd放在监听队列中，监听新的连接
    addEpollReadFd(_evtfd);
}

EventLoop::~EventLoop()
{
    if(_epfd)
    {
        close(_epfd);
    }
}


void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

// 该函数是在计算线程中通过TcpConnection调用的
// 目的是将要发送的信息交给IO线程，所以需要当前连接和要发送的消息
// 1. 将连接和消息（bind过来的send函数）放入待处理队列
// 2. 通过eventfd唤醒IO线程，将待处理队列中的任务都处理掉
// 通过以上两步，可以让不同线程处理同一连接中的信息传输
void EventLoop::runInLoop(Functor &&cb)
{
    // 使用大括号将某些栈变量/对象提前结束（提前解锁）
    {
        MutexLockGuard autoLock(_mutex);
        _pengingsCb.push_back(std::move(cb));
    }

    wakeup();
}

// 计算线程往evtfd中写入数据，唤醒IO线程
void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = ::write(_evtfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("write");
        return;
    }
}

void EventLoop::waitEpollFd()
{
    int nready = -1;
    do
    {
        nready = ::epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 5000);
    }while(-1 == nready && errno == EINTR);

    if(-1 == nready)
    {
        perror("nready == -1");
        return;
    }
    else if(0 == nready)
    {
        /* printf(">>epoll_wait timeout\n"); */
        // 定时唤醒后更新cache
        _cach->periodicUpdateCaches();
    }
    else// 有连接请求或有消息
    {
        // 监听就绪队列超过上限的情况，扩展监听就绪队列
        if(nready == (int)_evtList.size())
        {
            _evtList.resize(2 * nready);
        }
        // 遍历就绪队列
        for(int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            // 1. 有新连接请求进来
            if(fd == _acceptor.fd())
            {
                if(_evtList[idx].events & EPOLLIN)// 是写事件
                {
                    handleNewConnection();// 处理新连接
                }
            }
            // 2. 计算线程通知IO线程处理IO
            else if(fd == _evtfd)
            {
                handleRead();
                doPengingFunctors();
            }
            // 3. 已经建立的连接有数据发来
            else
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleMessage(fd);// 处理数据传输
                }
            }
        }
    }
}


// 处理新的连接请求
void EventLoop::handleNewConnection()
{
    // 1. 接收连接，返回peerfd证明三次握手完成
    int peerfd = _acceptor.accept();// accept函数中有判断是否正确
    // 2. 将新的连接加入监听中
    addEpollReadFd(peerfd);

    // 3. 构造新连接的对象，用智能指针的方式接收，能够自动释放
    // 通过该对象可以进行该连接上的数据传输
    TcpConnectionPtr con(new TcpConnection(peerfd, this));

    // Tcp连接创建可以进行三个事件的注册
    con->setConnectionCallback(_onConnectionCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    // 将新建立的连接的fd和连接信息记录，可以用con进行通讯
    _conns.insert(std::make_pair(peerfd, con));
    con->handleConnectionCallback();
}

// 处理IO操作
// 这步是为了判断一下连接是否建立成功
void EventLoop::handleRead()
{
    uint64_t one = 1;
    int ret = ::read(_evtfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("read");
        return;
    }
}

void EventLoop::doPengingFunctors()
{
    vector<Functor> tmp;
    {
        // 将待处理队列换出来，这样其他线程有操作可以往里放了
        // 减小锁的粒度
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pengingsCb);
    }

    // vector<Functor> _pengingsCb;// 队列中存的是每个回调函数，执行每个回调函数
    // _loop->runInLoop(std::bind(TcpConnection::send, this, msg));
    // 绑定的是是用当前连接发送信息的函数，遍历调用
    for(auto &cb : tmp)
    {
        cout << "2. send back!" << endl;
        cb();
    }
}

// 处理数据传输
void EventLoop::handleMessage(int fd)
{
    auto iter = _conns.find(fd);
    if(iter != _conns.end())
    {
        // 用Tcp连接里面定义一个函数
        // isClose，里面执行recv，recv的返回值等于0
        bool flag = iter->second->isClosed();// 判断是否关闭
        if(flag)
        {
            // 连接要断开了
            iter->second->handleCloseCallback();
            delEpollReadFd(fd);// 从监听队列中删除
            _conns.erase(iter);// 从已建立连接的队列中删除
        }
        else
        {
            iter->second->handleMessageCallback();
        }
    }
    else
    {
        // 连接不存在
        printf("该连接不存在\n");
    }
}

void EventLoop::setConnectionCallback(TcpConnectionCallback &&cb)
{
    _onConnectionCb = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMessageCb = std::move(cb);
}

void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onCloseCb = std::move(cb);
}


void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(-1 == ret)
    {
        perror("epoll_ctl_ADD");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(-1 == ret)
    {
        perror("epoll_ctl_DEL");
        return;
    }

}

int EventLoop::createEpollFd()
{
    int fd = epoll_create(100);// 参数没有意义
    if(-1 == fd)
    {
        perror("epoll_create");
        return -1;
    }
    return fd;
}

int EventLoop::createEventFd()
{
    int fd = eventfd(10, 0);
    if(-1 == fd)
    {
        perror("eventfd");
        return -1;
    }
    return fd;
}


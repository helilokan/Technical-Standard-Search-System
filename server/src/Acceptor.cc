#include "../include/Acceptor.h"
#include <stdio.h>


Acceptor::Acceptor(const string &ip, unsigned short port)
: _listenSock()// socket默认构造
, _servAddr(ip, port)
{

}

// 读取config文件建立连接
Acceptor::Acceptor(const string &filename)
: _listenSock()// socket默认构造
, _servAddr(filename)
{

}

Acceptor::~Acceptor()
{

}

// 设置为可复用，开始接收连接
void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}


void Acceptor::setReuseAddr()
{
    int on = 1;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(-1 == ret)
    {
        perror("setsockopt");
        return;
    }
}

void Acceptor::setReusePort()
{
    int on = 1;
    int ret = setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    if(-1 == ret)
    {
        perror("setsockopt");
        return;
    }
}

void Acceptor::bind()
{
    int ret = ::bind(_listenSock.fd(), 
                     (struct sockaddr *)_servAddr.getInetAddrPtr(), 
                     sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("bind");
        return;
    }
}

void Acceptor::listen()
{
    int ret = ::listen(_listenSock.fd(), 128);// 半连接队列大小为128
    if(-1 == ret)
    {
        perror("listen");
        return;
    }
}

// 建立连接成功，返回通信用的fd
int Acceptor::accept()
{
    int connfd = ::accept(_listenSock.fd(), nullptr, nullptr);
    if(-1 == connfd)
    {
        perror("listen");
        return -1;
    }
    return connfd;
}

// 返回等待连接的fd
int Acceptor::fd() const
{
    return _listenSock.fd();
}



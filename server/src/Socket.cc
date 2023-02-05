#include "../include/Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

// 默认构造
Socket::Socket()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_fd < 0)
    {
        perror("socket");
        return;
    }
}

// 有参构造
Socket::Socket(int fd)
: _fd(fd)
{

}

// 网络套接字需要自动释放，在类的析构函数内释放，RAII技术
Socket::~Socket()
{
    close(_fd);
}

// 获得文件描述符
int Socket::fd() const
{
    return _fd;
}

// 服务器有时需要关闭写端
void Socket::shuoDownWrite()
{
    // 关闭写端
    int ret = shutdown(_fd, SHUT_WR);
    if(ret)
    {
        perror("shutdown");
        return;
    }
}


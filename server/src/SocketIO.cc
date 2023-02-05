#include "../include/SocketIO.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


SocketIO::SocketIO(int fd)// 防止隐式转换
: _fd(fd)
{

}


SocketIO::~SocketIO()
{

}


int SocketIO::readn(char *buf, int len)
{
    int left = len;
    char *pstr = buf;
    int ret = 0;

    while(left > 0)
    {
        ret = read(_fd, pstr, left);
        if(-1 == ret && errno == EINTR)// 被打断
        {
            continue;
        }
        else if(-1 == ret)// 发生错误
        {
            perror("read error -1");
            return len - ret;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;// 指针偏移
            left -= ret;// 剩余读取内容减少
        }
    }

    return len - left;
}

int SocketIO::readLine(char *buf, int len)
{
    int left = len - 1;// 这里减了1留给结束符
    char *pstr = buf;
    int ret = 0, total = 0;

    while(left > 0)
    {
        // 不会将缓冲区中的数据清空
        ret = recv(_fd, pstr, left, MSG_PEEK);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("readLine error -1");
            return len - ret;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            for(int idx = 0; idx < ret; ++idx)
            {
                // 读到换行符，则就读到这里结束
                if(pstr[idx] == '\n')
                {
                    int sz = idx + 1;
                    readn(pstr, sz);// 读这一行
                    pstr += sz;
                    *pstr = '\0';

                    return total + sz;
                }
            }
            // 没有换行符，则将缓存中的都读出来
            readn(pstr, ret);
            total += ret;
            pstr += ret;
            left -= ret;
        }
    }
    *pstr = '\0';

    return total - left;
}

int SocketIO::writen(const char *buf, int len)
{
    int left = len;
    const char *pstr = buf;
    int ret = 0;

    while(left > 0)
    {
        ret = write(_fd, pstr, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            perror("writen error -1");
            return len - ret;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }

    return len - left;
}









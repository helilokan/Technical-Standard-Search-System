#include "../include/InetAddress.h"
#include <strings.h>


InetAddress::InetAddress(const string &ip, unsigned short port)
{
    ::bzero(&_addr, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

// 以sockaddr_in的参数类型构造
InetAddress::InetAddress(const struct sockaddr_in &addr)
: _addr(addr)
{

}

// 以配置文件的形式读入
InetAddress::InetAddress(const string &filename)
{
    // 通过读取文件名获取ip和端口号
    string ip;
    string port;
    readConfig(filename, ip, port);
    // 设置结构体
    ::bzero(&_addr, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(atoi(port.c_str()));
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

// 读config文件
void InetAddress::readConfig(const string &filename, string &ip, string &port)
{
    bool flag1 = false, flag2 = false;
    ifstream ifs(filename);
    if(!ifs.good())
    {
        perror("ifstream open");
        return;
    }
    
    string line;
    while(getline(ifs, line))
    {
        if(line.substr(0, 2) == "ip")
        {
            int i = 0;
            for(; i < line.size(); ++i)
            {
                if(isdigit(line[i]))
                {
                    break;
                }
            }
            ip = line.substr(i);
            flag1 = true;
        }
        else if(line.substr(0, 4) == "port")
        {
            int i = 0;
            for(; i < line.size(); ++i)
            {
                if(isdigit(line[i]))
                {
                    break;
                }
            }
            port = line.substr(i);
            flag2 = true;
        }
    }
    if(!flag1 || !flag2)
    {
        printf("config is wrong!\n");
        return;
    }
}


InetAddress::~InetAddress()
{

}

string InetAddress::ip() const
{
    return string(inet_ntoa(_addr.sin_addr));// 将c风格字符串转为c++风格的
}


unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port);// 网络字节序转为主机字节序（大端转小端）
}


const sockaddr_in * InetAddress::getInetAddrPtr() const
{
    return &_addr;
}









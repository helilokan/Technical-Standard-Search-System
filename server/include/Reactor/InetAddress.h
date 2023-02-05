#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <arpa/inet.h>
#include <string>
#include <fstream>

using std::ifstream;
using std::string;

class InetAddress
{
public:
    InetAddress(const string &ip, unsigned short port);
    InetAddress(const struct sockaddr_in &addr);
    InetAddress(const string &filename);
    ~InetAddress();
    void readConfig(const string &filename, string &ip, string &port);
    string ip() const;
    unsigned short port() const;
    const sockaddr_in *getInetAddrPtr() const;

private:
    struct sockaddr_in _addr;

};
#endif


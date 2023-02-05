#ifndef __Socket_H__
#define __Socket_H__

#include "NoCopyable.h"

class Socket: public NoCopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd() const;
    void shuoDownWrite();

private:
    int _fd;
};
#endif


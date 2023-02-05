#ifndef _NOCOPY_H
#define _NOCOPY_H

class NoCopyable
{
protected:
    NoCopyable() {}
    ~NoCopyable() {}
    NoCopyable(const NoCopyable &) = delete;
    NoCopyable &operator=(const NoCopyable &) = delete;
};
#endif


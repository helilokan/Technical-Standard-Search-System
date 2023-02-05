#include "../include/MutexLock.h"
#include <stdio.h>

MutexLock::MutexLock()
{
    int ret = pthread_mutex_init(&_mutex, nullptr);
    if(ret)
    {
        perror("pthread_mutex_init");
    }
}

MutexLock::~MutexLock()
{
    int ret = pthread_mutex_destroy(&_mutex);
    if(ret)
    {
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::Lock()
{
    int ret = pthread_mutex_lock(&_mutex);
    if(ret)
    {
        perror("pthread_mutex_lock");
    }
}

void MutexLock::unLock()
{
    int ret = pthread_mutex_unlock(&_mutex);
    if(ret)
    {
        perror("pthread_mutex_unlock");
    }
}






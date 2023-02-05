#include "../include/EchoServer.h"
#include "../include/CacheManager.h"
#include "../include/Configuration.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::istringstream;

const int THREAD_NUM = 3;
const int TASK_QUE = 10;

int test0()
{
    CacheManager *cach = CacheManager::getInstance();
    Configuration *conf = Configuration::getInstance();

    string ip = conf->getConfigMap()["ip"];
    istringstream iss(conf->getConfigMap()["port"]);
    unsigned short port;
    iss >> port;


    cout << "ip: " << ip << endl;
    cout << "port: " << port << endl;
    EchoServer echo(THREAD_NUM, TASK_QUE, ip, port, cach);
    echo.start();
}

int main()
{
    test0();
    return 0;
}


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtNetwork/QTcpSocket>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/socket.h>

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void socketConnected();
    void socketDisconnected();
    void socketReadyRead();

    void on_sendButton_clicked();

    void on_sendButton_2_clicked();

private:
    Ui::MainWindow *ui;
    bool connectedToHost;
    QTcpSocket* socket;
    void printMessage(QString message);
};

#endif // MAINWINDOW_H

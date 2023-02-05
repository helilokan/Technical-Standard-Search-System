#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectedToHost = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    if (!connectedToHost)
    {
        socket = new QTcpSocket();
        connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        //socket->connectToHost("127.0.0.1", 8888);
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8888);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        socklen_t length = sizeof(serverAddr);
        if (::connect(clientfd, (struct sockaddr *)&serverAddr, length) < 0)
        {
            perror("connect");
            ::close(clientfd);
            return;
        }
        socketConnected();

    }
    else
    {
        ::close(clientfd);
        socketDisconnected();
    }

}

void MainWindow::socketConnected()
{
    qDebug() << "Connected to server.";
    printMessage("<font color=\"Green\">Connected to server.</font>");
    ui->connectButton->setText("Disconnect");
    connectedToHost = true;
}

void MainWindow::socketDisconnected()
{
    qDebug() << "Disconnected from server.";
    printMessage("<font color=\"Red\">Disconnected from server.</font>");
    ui->connectButton->setText("Connect");
    connectedToHost = false;
}

void MainWindow::socketReadyRead()
{
    ui->chatDisplay->append(socket->readAll());
}

void MainWindow::on_sendButton_clicked()
{
     QString message = ui->messageInput->text();
     cout << ">> 请输入需要查询的内容: ";
     string line = message.toStdString();
     line = "1 " + line + "\n";
     send(clientfd, line.c_str(), line.size(), 0);
     char buf[65535] = {0};
     recv(clientfd, buf, sizeof(buf), 0);
     cout << buf << endl;
     string line2 = "你要检索的是：" + line ;
     printMessage(QString::fromStdString(line2));
     message = QString::fromStdString(buf);
     printMessage(message);
     ui->messageInput->clear();
}


void MainWindow::printMessage(QString message)
{
    ui->chatDisplay->append(message);
}

void MainWindow::on_sendButton_2_clicked()
{
    QString message = ui->messageInput->text();

    cout << ">> 请输入需要查询的内容: ";
    string line = message.toStdString();
    line = "2 " + line;
    line.append("\n");
    cout << line << endl;
    send(clientfd, line.c_str(), line.size(), 0);
    char buf[65535] = {0};
    recv(clientfd, buf, sizeof(buf), 0);
    cout << buf << endl;
    string line2 = "你要检索的是：" + line ;
    printMessage(QString::fromStdString(line2));
    message = QString::fromStdString(buf);
    printMessage(message);
    ui->messageInput->clear();
}

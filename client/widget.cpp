#include "mainwindow.h"
#include "widget.h"
#include "ui_widget.h"
#include<QMessageBox>
#include<QDebug>
#include<string>

using std::string;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    ui->id_line->setClearButtonEnabled(true);
    ui->passwordline->setClearButtonEnabled(true);
    ui->passwordline->setEchoMode(QLineEdit::Password);//设置密码格式
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_login_button_clicked()
{
       QString ID= ui->id_line->text();
       QString pa_wd=ui->passwordline->text();
       string ipNO = ID.toStdString();
       string wdNO = ID.toStdString();
       if(ipNO == "127.0.0.1" || wdNO == "8888")
       {
            QMessageBox::information(0,"提示","ip地址和端口号存在....正在连接");
            this->hide();
            main.show();
       }
       else
       {
            QMessageBox::information(0,"提示","No exist!");
       }

}

void Widget::on_register_button_clicked()
{
    //QMessageBox::information(0,"提示","正在申请账号，请按OK键");
    //这是测试简化
    //socket_com *s1=socket_com::getInstance();
    //s1->socket_start();
    //char tmpbuffer[1024];
    //int n=-1;
    //n=recv(s1->sockfd,tmpbuffer,1024,0);
   //if(n!=-1)
   //{
     //tmpbuffer[n]='\0';
    //qDebug()<<tmpbuffer;
    //QString str = QString(QLatin1String(tmpbuffer));
     //QMessageBox::information(0,"提示",str);
    //}
    //else
    //{
        //QMessageBox::warning(0,"警告","无法连接服务器");
    //}
}

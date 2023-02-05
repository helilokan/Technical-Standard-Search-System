一、untitled.pro构建工程文件

    1、该项目所调用的QT模块
    QT += core gui network
         核心 界面  网络

    2、该项目所有的cpp文件
    SOURCES += \
    main.cpp \
    mainwindow.cpp \
    widget.cpp
    
    3、该项目所有的h文件
    HEADERS += \
    mainwindow.h \
    widget.h

    4、该项目所有的ui文件
    FORMS += \
    mainwindow.ui \
    widget.ui

    \区分不同的文件，项目只会加载.pro里面的文件，如果没有写哪怕在同一个文件夹也不会加载

二、main.cpp程序入口函数
    仅用作窗口的启动

三、ui
    可以直接通过Qt Designer通过拖动图像化 构建会自动生成影响对应的.h 和 .cpp

四、关键函数
    绑定按键等空间与相应函数的启动
    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
             对象          信号          this           信号槽

    QMessageBox::information(0,"提示","ip地址和端口号存在....正在连接");
        Qt提供的一个提示窗口,可以不用自己设计ui

     QString message = ui->messageInput->text();
        获取输入栏控件中的文本
    Qstring与string的相互转换
        buf.toStdString();
        QString::fromStdString(buf);
    chatDisplay
        显示到输出栏等控件 

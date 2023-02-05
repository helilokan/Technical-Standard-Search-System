/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *nameInput;
    QPushButton *connectButton;
    QPushButton *sendButton;
    QTextBrowser *chatDisplay;
    QLineEdit *messageInput;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(429, 285);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        nameInput = new QLineEdit(centralwidget);
        nameInput->setObjectName(QString::fromUtf8("nameInput"));
        nameInput->setGeometry(QRect(60, 20, 270, 20));
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(340, 20, 75, 19));
        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setGeometry(QRect(330, 200, 75, 19));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sendButton->sizePolicy().hasHeightForWidth());
        sendButton->setSizePolicy(sizePolicy);
        chatDisplay = new QTextBrowser(centralwidget);
        chatDisplay->setObjectName(QString::fromUtf8("chatDisplay"));
        chatDisplay->setGeometry(QRect(20, 40, 331, 151));
        chatDisplay->setAcceptDrops(false);
        chatDisplay->setAcceptRichText(true);
        messageInput = new QLineEdit(centralwidget);
        messageInput->setObjectName(QString::fromUtf8("messageInput"));
        messageInput->setGeometry(QRect(20, 200, 301, 20));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 48, 19));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 429, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
#if QT_CONFIG(shortcut)
        label->setBuddy(messageInput);
#endif // QT_CONFIG(shortcut)

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
#if QT_CONFIG(tooltip)
        sendButton->setToolTip(QCoreApplication::translate("MainWindow", "Sends a message to other people", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        sendButton->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        sendButton->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
#if QT_CONFIG(tooltip)
        chatDisplay->setToolTip(QCoreApplication::translate("MainWindow", "Messages sent and received from other users", nullptr));
#endif // QT_CONFIG(tooltip)
        label->setText(QCoreApplication::translate("MainWindow", "Name\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

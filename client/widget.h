#ifndef WIDGET_H
#define WIDGET_H
#include "mainwindow.h"
#include <QWidget>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void on_login_button_clicked();

    void on_register_button_clicked();

private:
    Ui::Widget *ui;
    MainWindow main;
};

#endif // WIDGET_H

#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>
#include "calc.h"
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QDebug>
#include <QTcpSocket>

namespace Ui {
class Welcome;
}

class Welcome : public QMainWindow
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = nullptr);
    ~Welcome();

private slots:
    void on_btncalc_clicked();
    void update_fun();
private:
    Ui::Welcome *ui;
    //定义定时器对象
    QTimer timer;
};

#endif // WELCOME_H

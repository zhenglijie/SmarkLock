//锁界面
#include "welcome.h"
#include "ui_welcome.h"
#include "login.h"
#include <QTcpSocket>
#include <QtNetwork>
#include <QHostAddress>

Welcome::Welcome(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);

    //设定定时器时间
    timer.setInterval(1000);
    //关联槽函数 实现实时获取日期和时间
    connect(&timer,SIGNAL(timeout()),this,SLOT(update_fun()));
    timer.start();
}

Welcome::~Welcome()
{
    delete ui;
}

//跟新日期和时间
void Welcome::update_fun()
{
    //获取当前日期和时间
    QString ddate=QDate::currentDate().toString("yyyy-MM-dd");
    QString ttime=QTime::currentTime().toString("hh:mm:ss");

    //qDebug()<<ddate<<" "<<ttime;
    //显示数据到label
     ui->label_data->setText(ddate);
     ui->label_time->setText(ttime);
}
//计算器
void Welcome::on_btncalc_clicked()
{
    timer.stop();

    Calc *w=new Calc;
    w->show();
    this->hide();
}


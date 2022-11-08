//加载界面
#include "login.h"
#include <QApplication>
#include <QSplashScreen> //动画类
#include <QPixmap>//图片类
#include <QThread>//线程类
#include <QTcpSocket>
#include <QHostAddress>
#include <QtNetwork>
#include "function.h"
#include <QTimer>
#include "calc.h"
#include <QObject>
#include <QTimerEvent>

QString mainTemp;
int banTime = 0;
int banTimerID;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainTemp = "zhenglijie";
    //定义图片类
    QPixmap pix(":/photo/start.jpg");
    //定义动画类
    QSplashScreen splash(pix);
    //启动动画
    splash.show();
    //延时
    QThread::sleep(1);
    //关闭动画
    splash.close();

    Login w;
    w.show();

    return a.exec();
}

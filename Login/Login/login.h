#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QSqlDatabase> //数据驱动
#include <QSqlQuery> //数据库执行语句
#include <QCryptographicHash> //加密
#include <QMessageBox>//消息盒子
#include <QDebug>
#include <QDir>
#include <QTcpSocket>
#include <QtNetwork>
#include <QHostAddress>
#include <QTimer>

extern QString userNameNumber, userPasswd;

void sqlite_Init();

extern QTcpSocket *tcpClient;

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    QString username;
    QString password;
    void Hash_Init();
private slots:
    void on_btnlogin_clicked();

    void on_btnok_clicked();

    void on_offButton_clicked();

    void on_minButton_clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event);  //鼠标按下事件
    virtual void mouseMoveEvent(QMouseEvent *event);   //窗体拖动事件
    virtual void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件

private:
    QPoint mousePoint;      //鼠标相对于窗体的位置
    bool isMousePressed;    //鼠标是否按下
    Ui::Login *ui;
    bool isConnect = false;
    QHostAddress localIP = QHostAddress("192.168.43.63");
};

#endif // LOGIN_H

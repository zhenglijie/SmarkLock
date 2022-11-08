//登陆界面
#include "login.h"
#include "welcome.h"
#include "dialog.h"
#include "ui_login.h"
#include <QHostAddress>
#include <QtNetwork>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTimer>
#include <qlayout.h>
#include <QVBoxLayout>
#include <QTimerEvent>
#include <QGraphicsDropShadowEffect>
#include "inputiddevicedialog.h"
#include <QMouseEvent>
#include <QGraphicsView>

QTcpSocket *tcpClient;
QString Message;
QString userNameNumber, userPasswd;

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    sqlite_Init();//数据库初始化

    //设置窗体透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //设置无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(-3, 0);
    //设置阴影颜色
    shadow->setColor(QColor("#888888"));
    //设置阴影圆角
    shadow->setBlurRadius(30);
    //给嵌套QWidget设置阴影
    ui->lineuser->setGraphicsEffect(shadow);
    //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
    ui->lineuser->setContentsMargins(1,1,1,1);

    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow1->setOffset(-3, 0);
    //设置阴影颜色
    shadow1->setColor(QColor("#888888"));
    //设置阴影圆角
    shadow1->setBlurRadius(30);
    //给嵌套QWidget设置阴影
    ui->linepass->setGraphicsEffect(shadow1);
    //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
    ui->linepass->setContentsMargins(1,1,1,1);

    tcpClient = new QTcpSocket(this);
    //QHostAddress localIP = QHostAddress("192.168.43.63");
    tcpClient->connectToHost(localIP, 8891);
    tcpClient->write("QTID000001_LOGIN\n");
    if (tcpClient->waitForConnected(1000)) {
        isConnect = true;
        qDebug() << "connect successfully" << "\n";
    } else {
        qDebug() << "connect unsuccessfully" << "\n";
        //QMessageBox::information(this, "提示", "等待网络服务器连接...");
    }
}


////鼠标按下事件
void Login::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void Login::mouseMoveEvent(QMouseEvent *event)
{
    //若鼠标左键被按下
    if(isMousePressed == true)
   {

         //鼠标相对于屏幕的位置
         QPoint curMousePoint = event->globalPos() - mousePoint;
        //移动主窗体位置
         move(curMousePoint);
    }
    event->accept();
}

//鼠标释放事件
void Login::mouseReleaseEvent(QMouseEvent *event)
{
     //鼠标未按下
     isMousePressed = false;
     event->accept();
}

Login::~Login()
{
    delete ui;
}

void sqlite_Init()
{
    //创建目录对象
    QDir d;
    //创建路径
    QString dir="C:/Database/";
    d.mkpath(dir);
    //文件保存路径
    QString filenames=dir+"/"+"stu.db";
    qDebug()<<dir+"/"+"stu.db";
    //添加数据库驱动
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
    //指定数据库的路径
    db.setDatabaseName(filenames);
    //打开数据库
    if(!db.open())
    {
        qDebug()<<"open error";
    }
    //创建表格  //id 主键自增长   username 用户名不重复   password密码
    QString sql=QString("create table if not exists user(id integer primary key autoincrement,name ntext unique not NULL,username ntext unique not NULL,password ntext not NULL,deviceid integer)");
    //创建执行语句对象
    QSqlQuery query;
    //判断执行结果
    if(!query.exec(sql))
    {
        qDebug()<<"table create error";
    }
    else
    {
        qDebug()<<"table create success";
    }

    sql = QString("create table if not exists history(id integer primary key autoincrement, deviceid integer, time ntext, state ntext)");
    if (!query.exec(sql)) {
        qDebug() << "table2 create error";
    } else {
        qDebug() << "table2 create success";
    }
}

//获取输入框的内容并加密
void Login::Hash_Init()
{
     //获取界面输入框的内容
     username=ui->lineuser->text();
     password=ui->linepass->text();
     //将密码加密
     //创建加密对象
     QCryptographicHash hash(QCryptographicHash::Md5);
     //加入数据
     hash.addData(password.toUtf8());
     //获取加密数据
     QByteArray arr=hash.result();
     password=arr.toHex();
     qDebug()<<username;
     qDebug()<<password;
}
//注册
void Login::on_btnlogin_clicked()
{
    qDebug() << "hha" << "\n";
    if (!isConnect) {
        tcpClient->connectToHost(localIP, 8891);

        if (tcpClient->waitForConnected(50)) {
            qDebug() << "connect successfully!" << "\n";
            isConnect = true;
        } else {
            qDebug() << "connect unsuccessfully!" << "\n";
        }

        if (!isConnect) {
            QMessageBox::information(this, "提示", "网络未连接!");
            return;
        }
    }

    Dialog *w=new Dialog;
      w->show();
     // this->hide();
}
//登录
void Login::on_btnok_clicked()
{
    if (!isConnect) {
        tcpClient->connectToHost(localIP, 8891);

        if (tcpClient->waitForConnected(50)) {
            qDebug() << "connect successfully!" << "\n";
            isConnect = true;
        } else {
            qDebug() << "connect unsuccessfully!" << "\n";
        }

        if (!isConnect) {
            QMessageBox::information(this, "提示", "网络未连接!");
            return;
        }
    }

    userNameNumber = ui->lineuser->text();
    userPasswd = ui->linepass->text();

    sqlite_Init();//数据库初始化
    //执行加密函数
    Hash_Init();
    //对比账户和密码
    //将加密数据加入到数据库
    QString sql=QString("select * from user where username='%1' and password='%2'")
            .arg(username).arg(password);
    //创建执行语句对象
    QSqlQuery query(sql);
    //判断执行结果
    if(!query.next())
    {
        qDebug()<<"Login error";
        QMessageBox::information(this,"登录认证","登录失败,账户或者密码错误");
    }
    else
    {

        qDebug()<<"Login success";
        QMessageBox::information(this,"登录认证","登录成功");
        //播放内容
        QString name="登录成功，欢迎用户"+username+"进入";

//        Welcome *w=new Welcome;
//        w->show();
//        this->close();


        //inputIdDeviceDialog *w = new inputIdDeviceDialog;
        Calc *w=new Calc;
        w->show();
        this->close();
    }
}

void Login::on_offButton_clicked()
{
    if (isConnect) {
        tcpClient->disconnectFromHost();
        tcpClient->close();
    }

    this->close();
}

void Login::on_minButton_clicked()
{
    this->showMinimized();
}

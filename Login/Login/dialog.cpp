//注册界面
#include "dialog.h"
#include "login.h"
#include "ui_dialog.h"
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
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
        ui->label->setGraphicsEffect(shadow);
        //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
        ui->label->setContentsMargins(1,1,1,1);

        this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        //实例阴影shadow
        QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect(this);
        //设置阴影距离
        shadow1->setOffset(-3, 0);
        //设置阴影颜色
        shadow1->setColor(QColor("#888888"));
        //设置阴影圆角
        shadow1->setBlurRadius(30);
        //给嵌套QWidget设置阴影
        ui->label_2->setGraphicsEffect(shadow1);
        //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
        ui->label_2->setContentsMargins(1,1,1,1);

        this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        //实例阴影shadow
        QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect(this);
        //设置阴影距离
        shadow2->setOffset(-3, 0);
        //设置阴影颜色
        shadow2->setColor(QColor("#888888"));
        //设置阴影圆角
        shadow2->setBlurRadius(30);
        //给嵌套QWidget设置阴影
        ui->label_3->setGraphicsEffect(shadow2);
        //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
        ui->label_3->setContentsMargins(1,1,1,1);

        this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        //实例阴影shadow
        QGraphicsDropShadowEffect *shadow3 = new QGraphicsDropShadowEffect(this);
        //设置阴影距离
        shadow3->setOffset(-3, 0);
        //设置阴影颜色
        shadow3->setColor(QColor("#888888"));
        //设置阴影圆角
        shadow3->setBlurRadius(30);
        //给嵌套QWidget设置阴影
        ui->label_5->setGraphicsEffect(shadow3);
        //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
        ui->label_5->setContentsMargins(1,1,1,1);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void Dialog::mouseMoveEvent(QMouseEvent *event)
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
void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
     //鼠标未按下
     isMousePressed = false;
     event->accept();
}

//插入
void Dialog::signUp()
{
        //将加密数据加入到数据库
        QString sql=QString("insert into user(name,username,password) values('%1','%2', '%3')")
                .arg(user).arg(username).arg(password);
        //创建执行语句对象
        QSqlQuery query;
        //判断执行结果
        if(!query.exec(sql))
        {
            qDebug() << user << " " << username << password << "\n";
            qDebug()<<"insert into error";
            QMessageBox::information(this,"注册认证","注册失败！");
        }
        else
        {
            //初始化文字转语音
            qDebug()<<"insert into success";
            QMessageBox::information(this,"注册认证","注册成功！");
            this->close();
        }

}

//注册
void Dialog::on_btnok_clicked()
{
    sqlite_Init();
    //获取界面输入框的内容

    user=ui->linename->text();
    if (!user.size()) {
        QMessageBox::information(this, "提示", "用户名不能为空！");
        return;
    }

    username=ui->lineusername->text();
    if (!username.size()) {
        QMessageBox::information(this, "提示", "账号不能为空！");
        return;
    }

    QRegExp reg1("^[0-9a-zA-Z]{5,9}$");
    qDebug() << "username = " << username << "\n";
    if (!reg1.exactMatch(username.trimmed())) {
        qDebug() << "请输入由数字大小写字母组成的5~9位的账号";
        QMessageBox::information(this, "提示", "请输入由数字大小写字母组成的5~9位账号");
        return;
    }

    password=ui->linepassword->text();

    QRegExp reg("^[0-9a-zA-Z]{8,20}$");
    qDebug() << "password = " << password << "\n";
    if (!reg.exactMatch(password.trimmed())) {
        qDebug() << "请输入由数字大小写字母组成的8~20位密码";
        QMessageBox::information(this, "提示", "请输入由数字大小写字母组成的8~20位密码");
        //QMessageBox::information(this,"注册认证","注册失败！");
        return;
    }

    pass=ui->linepassword_ok->text();

    //判断密码和确认密码是否一样
    if(password == pass)
    {
        //将密码加密
        //创建加密对象
        QCryptographicHash hash(QCryptographicHash::Md5);
        //加入数据
        hash.addData(password.toUtf8());
        //获取加密数据
        QByteArray arr=hash.result();
        password=arr.toHex();
        qDebug()<<password;
        signUp();
    }
    else
    {
        QMessageBox::information(this,"注册认证","两次密码输入不一致");

    }
}
//跳转
void Dialog::on_btnlev_clicked()
{


    //Login *w=new Login;
    //w->show();
    this->close();
}

void Dialog::on_offButton_clicked()
{
    this->close();
}

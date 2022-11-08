//动态密码
//动态密码
#include "calc.h"
#include "welcome.h"
#include "ui_calc.h"
#include <QString>
#include <QDialog>
#include <QInputDialog>
#include <qdir.h>
#include <QProgressBar>
#include "login.h"
#include <string>
#include "function.h"
#include <QTimerEvent>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QInputDialog>
#include <QRegExp>
#include <QList>
#include <QPixmap>
#include <QMouseEvent>
#include <QPaintEvent>

Calc::Calc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calc)
{
    ui->setupUi(this);

    ui->viewlabel->setPixmap(QPixmap(":/photo/lock.png"));

    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    qDebug() << "Number = " << userNameNumber << "\n";
    QString sql1 = QString("select name from user where username = '%1'").arg(userNameNumber);
    QString sql2 = QString("select deviceid from user where username = '%1'").arg(userNameNumber);

    QSqlQuery query1(sql1);
    QSqlQuery query2(sql2);
    QSqlRecord rec1 = query1.record();
    QSqlRecord rec2 = query2.record();
    if (query1.next()) {
        int col = rec1.indexOf("name");
        userName = query1.value(col).toString();
        qDebug() << "success " << userName << "\n";
        ui->username->setText("您好，亲爱的用户：" + userName);
    } else {
        qDebug() << "fail " << userName << "\n";
        ui->username->setText("您好，亲爱的用户：" + userName);
    }

    ui->username->setText("您好，亲爱的用户：" + userName);

    if (query2.next()) {
        int col = rec2.indexOf("deviceid");
        deviceId = query2.value(col).toString();
        qDebug() << "当前id:" << deviceId << "\n";
        if (deviceId == "") ui->lockID->setText("未查询到您的设备ID!");
        else ui->lockID->setText("当前绑定设备ID为：" + deviceId);
    } else {
        ui->lockID->setText("未查询到您的设备ID!");
        deviceId = "";
        //QMessageBox::information()
    }

    //int id = deciveId;
    qDebug() << "toint = " << deviceId.toInt() << "\n";

    qDebug() << "size = " << deviceId.size() << "\n";

    if (deviceId.size()) {
        qDebug() << deviceId.toInt() << "\n";
        QString sql3 = QString("select * from history where deviceid = %1").arg(613323);
        QSqlQuery query3(sql3);
        QSqlRecord rec3 = query3.record();
        while (query3.next()) {
            qDebug() << "1" << "\n";
            int col = 4;
            QList<QString> temp;
            for (int i = 1; i < col; i++) {
                QString value = query3.value(i).toString();
                temp.push_back(value);
                qDebug() << value << "";
            }
            History.push_front(temp);
            qDebug() << "\n";
        }

        for (int i = 0; i < History.size(); i++) {
            for (int j = 0; j < History[i].size(); j++) {
                qDebug() << History[i][j];
            }
            qDebug() << "\n";
        }
    }

    //设置开锁记录，table Widget

    ui->tableWidget->setRowCount(100);
    ui->tableWidget->setColumnCount(3);
   // ui->tableWidget->setColumnWidth(0, 70);  //设置列宽
    ui->tableWidget->setColumnWidth(0, 100);  //设置列宽
    ui->tableWidget->setColumnWidth(1, 220);  //设置列宽
    ui->tableWidget->setColumnWidth(2, 70);  //设置列宽

    QStringList strList;
    strList << tr("设备ID")<<tr("日期")<<tr("状态");
    ui->tableWidget->setHorizontalHeaderLabels(strList);
    for (int i = 0; i < History.size(); i++) {
        for (int j = 0; j < History[i].size(); j++) {
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(History[i][j]));
        }
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(30);

    prog_num = 0;
    myTimer = new QTimer(this);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(process()));
    myTimer->start(1000);

    if (banTime > 0) banTimerID = startTimer(1000);

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

//鼠标按下事件
void Calc::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void Calc::mouseMoveEvent(QMouseEvent *event)
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
void Calc::mouseReleaseEvent(QMouseEvent *event)
{
     //鼠标未按下
     isMousePressed = false;
     event->accept();
}


void Calc::timerEvent(QTimerEvent *e) {
    if (banTime >= 0) {
        qDebug() << "定时器：" << banTimerID << " " << " 倒计时：" << banTime;
        ui->stateLabel->setText("禁用 " + QString::number(banTime) + "S");
        banTime--;
    } else {
        ui->stateLabel->setText("LOCK");
        killTimer(e->timerId());
        banTime = 0;
    }
}

void Calc::updateRecord(QString dvid) {
    qDebug() << "1 = " << dvid << "\n";

    if (!dvid.size()) return;

    QString sql3 = QString("select * from history where deviceid = %1").arg(dvid.toInt());
    //QString sql3 = QString("select * from history");
    QSqlQuery query3(sql3);
    QSqlRecord rec3 = query3.record();
    while (query3.next()) {
        //qDebug() << "1" << "\n";
        int col = 4;
        QList<QString> temp;
        for (int i = 1; i < col; i++) {
            QString value = query3.value(i).toString();
            temp.push_back(value);
            qDebug() << value << "";
        }
        History.push_front(temp);
        qDebug() << "\n";
    }

    //for (int i = 0; i < )

    for (int i = 0; i < History.size(); i++) {
        for (int j = 0; j < History[i].size(); j++) {
            qDebug() << History[i][j] << "";
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(History[i][j]));
        }
        qDebug() << "\n";
    }
}

void Calc::readMessage() {
    if (!deviceId.size()) {
        return;
    }
    QByteArray array;
    array = tcpClient->readAll();
    QString message = QString(array);
    if (!message.size()) return;

    QString sql4;

    curDateTime = QDateTime::currentDateTime();
    QString time = curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "recv message:  " << message << "\n";
    if (message == "SUCC") {
        sql4 = QString("insert into history(deviceid,time,state) values('%1','%2','%3')")
                .arg(deviceId.toInt()).arg(time).arg(message);
        if (historyQuery.exec(sql4)) {
            qDebug() << "插入成功" << "\n";
        }
        ui->viewlabel->setPixmap(QPixmap(":/photo/unlock.png"));
        ui->stateLabel->setText("OPEN");
        updateRecord(deviceId);
    } else if (message == "FAIL") {
        sql4 = QString("insert into history(deviceid,time,state) values('%1','%2','%3')")
                .arg(deviceId.toInt()).arg(time).arg(message);
        if (historyQuery.exec(sql4)) {
            qDebug() << "插入成功" << "\n";
        }
        updateRecord(deviceId);
    } else if (message == "BANW") {
        //ui->stateLabel->setText("BANW");
        banTime = 3; //Test
        banTimerID = startTimer(1000);
        sql4 = QString("insert into history(deviceid,time,state) values('%1','%2','%3')")
                .arg(deviceId.toInt()).arg(time).arg(message);
        historyQuery.exec(sql4);
        updateRecord(deviceId);
    } else if (message == "BANX") {
        //ui->stateLabel->setText("BANX");
        banTime = 6;
        banTimerID = startTimer(1000);
        sql4 = QString("insert into history(deviceid,time,state) values('%1','%2','%3')")
                .arg(deviceId.toInt()).arg(time).arg(message);
        historyQuery.exec(sql4);
        updateRecord(deviceId);
    } else if (message == "BANY") {
        //ui->stateLabel->setText("BANY");
        banTime = 9;
        banTimerID = startTimer(1000);
        sql4 = QString("insert into history(deviceid,time,state) values('%1','%2','%3')")
                .arg(deviceId.toInt()).arg(time).arg(message);
        historyQuery.exec(sql4);
        updateRecord(deviceId);
    } else if (message == "BANZ") {
        //ui->stateLabel->setText("BANZ");
        banTime = 2147483647;
        banTimerID = startTimer(1000);
        sql4 = QString("insert into history(deviceid,time,state) values('%1','%2','%3')")
                .arg(deviceId.toInt()).arg(time).arg(message);
        historyQuery.exec(sql4);
        updateRecord(deviceId);
    } else if (message == "LOCK") {
        ui->viewlabel->setPixmap(QPixmap(":/photo/lock.png"));
        ui->stateLabel->setText("LOCK");
        sql4 = QString("insert into history(deviceid,time,state) values('%1','%2','%3')")
                .arg(deviceId.toInt()).arg(time).arg(message);
        historyQuery.exec(sql4);
        updateRecord(deviceId);
    } else if (message == "havebind") {
        QMessageBox::information(this, "message", "已经绑定");
    } else if (message == "bindsucc") {
        QMessageBox::information(this, "message", "绑定成功");
    } else if (message == "bindfailed") {
        QMessageBox::information(this, "message", "绑定失败");
    }
}

Calc::~Calc() //释放更新密码的计时器
{
    if (myTimer) {
        if (myTimer->isActive()) {
            myTimer->stop();
        }
    }
    delete myTimer;
    myTimer = nullptr;

    if (newTimer) {
        if (newTimer->isActive()) {
            newTimer->stop();
        }
    }
    delete newTimer;
    newTimer = nullptr;

    delete ui;
}

void Calc::on_pushButton_clicked()
{
    bool isOK;
    QString text = QInputDialog::getText(NULL, "Input Dialog",
                                                       "请输入您的设备ID(六位整数)",
                                                       QLineEdit::Normal,
                                                       "",
                                                       &isOK);


    if(isOK) {
        QString str = "^[0-9]{6}$";
        QRegExp rx(str);
        if (rx.exactMatch(text.trimmed())) {
//            QMessageBox::information(NULL, "Information",
//                                               "Your comment is: <b>" + text + "</b>",
//                                               QMessageBox::Yes,
//                                               QMessageBox::Yes);

            ui->lockID->setText("当前绑定设备ID为：" + text);
            //ui->lockID->setText(text);


            std::string res = "QTID000001_AR" + text.toStdString();
            //tcpClient->write(res.c_str());
            QString sql = QString("update user set deviceid = %1 where id = %2").arg(text.toInt()).arg(1);
            QSqlQuery query;
            if (query.exec(sql)) {
                qDebug() << "更新成功!" << "\n";
            } else {
                qDebug() << "更新失败!" << "\n";
            }
            //QTID000001_ARXXXXXX_PSXXXXXX
            QString pwd = ui->randLabel->text();
            res += "PS" + pwd.toStdString();
            qDebug() << "sendInfo:" << res.c_str() << "\n";
            tcpClient->write(res.c_str());
            deviceId = text;
            qDebug() << "text = " << text << "\n";
            qDebug() << "deviceId = " << deviceId << "\n";
            updateRecord(text);
           // QString sql = QString("update user set password = '%1' where id = %2").arg(text.toInt()).arg(1);
//            QSqlQuery query;
//            if (query.exec(sql)) {
//                qDebug() << "更新成功!" << "\n";
//            } else {
//                qDebug() << "更新失败!" << "\n";
//            }

        } else {
            QMessageBox::information(NULL, "Information",
                                               "输入格式错误！",
                                               QMessageBox::Yes | QMessageBox::No,
                                               QMessageBox::Yes);

        }

    } else {
        qDebug() << "点击取消" << "\n";

    }

   //qDebug() << text << "\n";
}



int Calc::generateRandomInteger(int min, int max) //产生随机数
{
    Q_ASSERT(min < max);
    // 加入随机种子。种子是当前时间距离0点0分0秒的秒数。
    // 每次启动程序，只添加一次种子，以做到数字真正随机。
    static bool seedStatus;
    if (!seedStatus)
    {
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        seedStatus = true;
    }
    int nRandom = qrand() % (max - min);
    nRandom = (min * (qrand() % 9 + 1)) + nRandom;
    return nRandom;
}

void Calc::process()
{
    tim ++;
    if(tim>30)
    {
        tim=0;
        ui->progressBar->setValue(0);

        int nRand = generateRandomInteger(100000, 999999);
        QString strRand = QString::number(nRand);
        std::string res = "QTID000001_UPDPS" + strRand.toStdString();
        qDebug() << "sendInfo:" << res.c_str() << "\n";

        if (deviceId.size())
            tcpClient->write(res.c_str());

        ui->randLabel->setText(strRand);
    }
    else {
        ui->progressBar->setValue(tim);
    }
}

void Calc::on_offButton_clicked()
{
    tcpClient->disconnectFromHost();
    tcpClient->close();
    this->close();
}

//void Calc::paintEvent(QPaintEvent *event)
//{
//    QPainter p(this);
//    p.drawPixmap(0, 0,400,300, QPixmap(":/photo/3.jpg"));
//}
//void Calc::mouseMoveEvent(QMouseEvent *event)
//{
//    if (event->buttons() & Qt::LeftButton) //按下鼠标的左键
//    {
//        move(event->globalPos() - p);
//    }
//}
//void Calc::mousePressEvent(QMouseEvent *event)
//{
//    //右键关闭，左键移动
//    if (event->button() == Qt::RightButton)
//    {
//        close();
//    }
//    else if (event->button() == Qt::LeftButton)
//    {
//        //求坐标差值  当前点击坐标 - 窗口左上角坐标
//        p = event->globalPos() - this->frameGeometry().topLeft();
//    }
//}


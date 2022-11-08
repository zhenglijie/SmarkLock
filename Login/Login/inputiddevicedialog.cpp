#include "inputiddevicedialog.h"
#include "ui_inputiddevicedialog.h"
#include "login.h"
#include <string>
#include <QString>
#include <QTcpSocket>
#include "calc.h"
#include <QByteArray>
#include "workthread.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QTcpSocket>

inputIdDeviceDialog::inputIdDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputIdDeviceDialog)
{
    ui->setupUi(this);
//    Calc *w=new Calc();
//    w->show();
//    this->close();

    //WorkThread *task = new WorkThread;
    //qDebug() << "线程开始" << "\n";
    //task->start();
//    QString sql = QString("select deviceid from user where id = 1");
//    QSqlQuery query(sql);
//    QSqlRecord rec = query.record();

//    if (query.next()) {
//        int col = rec.indexOf("deviceid");
//        if (query.value(col).toString() != "") {
//            haveRecvFlag = true;
//        }
//        //ui->username->setText("您好，亲爱的用户：" + userName);
//    }
//    socket = new QTcpSocket(this);
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

void inputIdDeviceDialog::readMessage() {
    if (haveRecvFlag) return;
    QByteArray array;
    if (!haveRecvFlag) array = tcpClient->readAll();
    message = QString(array);
    if (!message.size()) return;

    haveRecvFlag = true;

    qDebug() << message << "\n";
    if (message == "havebind") {
        QMessageBox::information(this, "message", "已经绑定");
        Calc *w=new Calc();
        w->show();
        this->close();
    } else if (message == "bindsucc") {
        QMessageBox::information(this, "message", "绑定成功");
        Calc *w=new Calc();
        w->show();
        this->close();
    } else if (message == "bindfailed") {
        QMessageBox::information(this, "message", "绑定失败");
    }
}


inputIdDeviceDialog::~inputIdDeviceDialog()
{
    delete ui;
}

void inputIdDeviceDialog::on_btnCancel_clicked()
{

    this->close();
}

void inputIdDeviceDialog::on_btnSure_clicked()
{
    inputId = ui->input->text();
    QString get = ui->input->text();
    qDebug() << "edit = " << get << "\n";

    std::string res = "QTID000001_AR" + get.toStdString();
    tcpClient->write(res.c_str());
    QString sql = QString("update user set deviceid = %1 where id = %2").arg(inputId.toInt()).arg(1);
    QSqlQuery query;
    if (query.exec(sql)) {
        qDebug() << "更新成功!" << "\n";
    } else {
        qDebug() << "更新失败!" << "\n";
    }
}

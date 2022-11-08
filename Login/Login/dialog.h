#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    virtual void mousePressEvent(QMouseEvent *event);  //鼠标按下事件
    virtual void mouseMoveEvent(QMouseEvent *event);   //窗体拖动事件
    virtual void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件

    void signUp();
private slots:
    void on_btnok_clicked();

    void on_btnlev_clicked();

    void on_offButton_clicked();


private:
    Ui::Dialog *ui;
    QString user;
    QString username;
    QString password;
    QString pass;
    QPoint mousePoint;      //鼠标相对于窗体的位置
    bool isMousePressed;    //鼠标是否按下
};

#endif // DIALOG_H

#ifndef CALC_H
#define CALC_H

#include <QMainWindow>
#include <QLabel> //标题
#include <QLineEdit>//输入框
#include <QPushButton>//按钮
#include <QVBoxLayout>//垂直布局
#include <QGridLayout>//栅格布局
#include <QDebug>
#include <QMessageBox>//消息盒子
#include <QTimerEvent>
#include <QSqlQuery>
#include <QDateTime>
#include <QList>

//extern void updateStateLabel();
//void updateStateLabel();

namespace Ui {
class Calc;
}

class Calc : public QMainWindow
{
    Q_OBJECT

private:
      QLabel *title;//标题
      QLineEdit *line;//输入框
      QPushButton *button[16];//按钮
      QVBoxLayout *vbox;//垂直对象
      QGridLayout *gbox;//栅格对象

      void window_Init();//窗体显示
      void Key_Init();//按键显示
      void Font_Init();//字体初始化
      void inputNum(QString ch);
      void inputType(QString ch);
      void output();

      int calc=0;
      QString firstnum;
      QString secondnum;

public:
    explicit Calc(QWidget *parent = nullptr);
    ~Calc();
    int generateRandomInteger(int min, int max);
    void timerEvent(QTimerEvent *e);
    void updateRecord(QString dvid);

    //鼠标是否按下属性
    bool m_bPressed=false;

    //按下后当前鼠标位置属性
    QPoint m_point;

protected:
    virtual void mousePressEvent(QMouseEvent *event);  //鼠标按下事件
    virtual void mouseMoveEvent(QMouseEvent *event);   //窗体拖动事件
    virtual void mouseReleaseEvent(QMouseEvent *event);//鼠标释放事件


private slots:
      void on_pushButton_clicked();

      void  process();

      void readMessage();
     // void updateStateLabel();
//      void dealRemain();
      void on_offButton_clicked();

private:
      QPoint mousePoint;      //鼠标相对于窗体的位置
      bool isMousePressed;    //鼠标是否按下
      Ui::Calc *ui;

      int prog_num;
      QTimer *myTimer;
      int tim = 31;

      QTimer *newTimer;
      bool changFlag = false;

      QTimer *banTimer;

      QString userName;
      QString deviceId;

      QString getInputId;

      QDateTime curDateTime;

      QSqlQuery historyQuery;

      QList<QList<QString>> History;

      //QPoint p;
};

#endif // CALC_H

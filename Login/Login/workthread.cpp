#include "workthread.h"
#include <QDebug>

WorkThread::WorkThread()
{
    workCount = 0;
}

void WorkThread::run() {
    while (true) {
        workCount++;
        if(workCount>=1000) workCount=0;
        qDebug()<<"WorkThread"<<workCount;
        sleep(1);//线程 睡眠一秒一次
    }
}

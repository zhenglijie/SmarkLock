#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QThread>


class WorkThread:public QThread
{
public:
    WorkThread();
    int workCount;
private:
    virtual void run();
};

#endif // WORKTHREAD_H

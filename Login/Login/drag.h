#ifndef DRAG_H
#define DRAG_H

#include <QWidget>
#include <QMouseEvent>

class drag: public QWidget
{
    Q_OBJECT

public:
    drag(QWidget *parent = 0);

    bool m_bPressed = true;
    QPoint m_point;

    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif

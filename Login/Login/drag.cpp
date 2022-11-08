//drag.cpp


#include "drag.h"

drag::drag(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setFixedSize(100, 100);        //设置窗口大小
}

void drag::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bPressed)
        move(event->pos() - m_point + pos());
}

void drag::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

void drag::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
    }
}

#include "mybutton.h"
#include <QMouseEvent>

MyButton::MyButton(QWidget* parent)
    : QPushButton{parent}
{
}

void MyButton::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        m_pixmap.load(m_pressed);
        update();
    }
    QPushButton::mousePressEvent(ev);
}

void MyButton::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        m_pixmap.load(m_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(ev);
}

void MyButton::enterEvent(QEnterEvent* ev)
{
    m_pixmap.load(m_hover);
    update();
    QPushButton::enterEvent(ev);
}

void MyButton::leaveEvent(QEvent* ev)
{
    m_pixmap.load(m_normal);
    update();
    QPushButton::leaveEvent(ev);
}

void MyButton::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
}

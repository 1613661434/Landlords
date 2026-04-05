#include "cardpanel.h"
#include <QMouseEvent>

CardPanel::CardPanel(QWidget* parent)
    : QWidget{parent}, m_isFront(true)
{
}

void CardPanel::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_isFront ? m_front : m_back);
}

void CardPanel::mousePressEvent(QMouseEvent* ev)
{
    emit cardSelected(ev->button());
}

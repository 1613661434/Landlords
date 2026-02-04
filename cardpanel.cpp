#include "cardpanel.h"

CardPanel::CardPanel(QWidget* parent)
    : QWidget{parent}, m_isFront(true)
{
}

void CardPanel::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_isFront ? m_front : m_back);
}

void CardPanel::mousePressEvent(QMouseEvent* event)
{
}

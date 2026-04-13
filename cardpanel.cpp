#include "cardpanel.h"
#include <QMouseEvent>

CardPanel::CardPanel(QWidget* parent)
    : QWidget{parent}, m_isFront(true), m_isSelected(false), m_owner(nullptr)
{
}

void CardPanel::paintEvent(QPaintEvent* ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(), m_isFront ? m_front : m_back);
}

void CardPanel::mousePressEvent(QMouseEvent* ev)
{
    emit cardSelected(ev->button());
}

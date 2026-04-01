#include "animationwindow.h"
#include <QTimer>
#include <QPainter>

AnimationWindow::AnimationWindow(QWidget* parent)
    : QWidget{parent}
{
}

void AnimationWindow::showBetScore(int point)
{
    m_x = 0;
    switch (point)
    {
    case 1:
        m_image.load(":/images/score1.png");
        break;
    case 2:
        m_image.load(":/images/score2.png");
        break;
    case 3:
        m_image.load(":/images/score3.png");
        break;
    default:
        break;
    }
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::paintEvent(QPaintEvent* ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(m_x, 0, m_image.width(), m_image.height(), m_image);
}

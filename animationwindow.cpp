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

void AnimationWindow::showSequence(bool isPair)
{
    m_x = 0;
    QString name = isPair ? ":/images/liandui.png" : ":/images/shunzi.png";
    m_image.load(name);
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::showBomb()
{
    m_x = 0;
    m_index = 0;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]()
            {
        ++m_index;
        if(m_index > 12)
        {
            timer->stop();
            timer->deleteLater();
            hide();
            return;
        }
        QString name = QString(":/images/bomb_%1.png").arg(m_index);
        m_image.load(name);
        update(); });
    timer->start(60);
}

void AnimationWindow::showJokerBomb()
{
    m_x = 0;
    m_index = 0;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer]()
            {
        ++m_index;
        if(m_index > 8)
        {
            timer->stop();
            timer->deleteLater();
            hide();
            return;
        }
        QString name = QString(":/images/joker_bomb_%1.png").arg(m_index);
        m_image.load(name);
        update(); });
    timer->start(60);
}

void AnimationWindow::showPlane()
{
    m_x = width();
    m_image.load(":/images/plane_1.png");
    setFixedHeight(m_image.height());
    update();

    const int step = width() / 5;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer, step]()
            {
        static int dist = 0;
        static int timers = 0;
        dist += 5;
        m_x -= 5;
        if(m_x <= -110)
        {
            timer->stop();
            timer->deleteLater();
            dist = timers = 0;
            hide();
            return;
        }
        if(dist >= step)
        {
            dist = 0;
            ++timers;
            QString name = QString(":/images/plane_%1.png").arg(timers % 5 + 1);
            m_image.load(name);
        }
        update(); });
    timer->start(15);
}

void AnimationWindow::paintEvent(QPaintEvent* ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(m_x, 0, m_image.width(), m_image.height(), m_image);
}

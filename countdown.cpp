#include "countdown.h"
#include <QPainter>

CountDown::CountDown(QWidget* parent)
    : QWidget{parent}
{
    setFixedSize(70, 70);
    m_timer = new QTimer(this);
    m_emptyPixmap = QPixmap();
    m_clock.load(":/images/clock.png");
    m_numberBase.load(":/images/number.png");

    connect(m_timer, &QTimer::timeout, this, [=]()
            {
        --m_count;

        if (m_count > 9) return;
        
        if (m_count > 0)
        {
            m_number = m_numberBase.copy(m_count*40, 0, 30, 42).scaled(20, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            if (m_count == 5) emit notMuchTime();
        } 
        else
        {
            m_clock = m_emptyPixmap;
            m_number = m_emptyPixmap;
            m_timer->stop();
            emit timeout();
        }
        update(); });
}

void CountDown::showCountDown()
{
    m_count = 15;
    m_clock.load(":/images/clock.png");
    m_number = m_emptyPixmap;
    m_timer->start(1000);
}

void CountDown::stopCountDown()
{
    m_timer->stop();
    m_clock = m_emptyPixmap;
    m_number = m_emptyPixmap;
    update();
}

void CountDown::paintEvent(QPaintEvent* ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(), m_clock);
    p.drawPixmap(24, 24, m_number.width(), m_number.height(), m_number);
}

#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QWidget>
#include <QTimer>

class CountDown : public QWidget
{
    Q_OBJECT
private:
    static const QPixmap& emptyPixmap();
    QPixmap m_clock;
    QPixmap m_numberBase;
    QPixmap m_number;
    QTimer* m_timer = nullptr;
    int m_count = 0;

public:
    explicit CountDown(QWidget* parent = nullptr);

    void showCountDown();
    void stopCountDown();

protected:
    void paintEvent(QPaintEvent* ev) override;

signals:
    void notMuchTime();
    void timeout();
};

#endif // COUNTDOWN_H

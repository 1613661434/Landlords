#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

class Loading : public QWidget
{
    Q_OBJECT
private:
    QPixmap m_image;
    QPixmap m_progress;
    int m_dist = 0;

public:
    explicit Loading(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* ev) override;

signals:
};

#endif // LOADING_H

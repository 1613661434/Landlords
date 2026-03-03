#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QPainter>

class MyButton : public QPushButton
{
    Q_OBJECT
private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QPixmap m_pixmap;

public:
    explicit MyButton(QWidget* parent = nullptr);

    inline void setImage(QString normal, QString hover, QString pressed)
    {
        m_normal = normal;
        m_hover = hover;
        m_pressed = pressed;
        m_pixmap.load(m_normal);
        update();
    }

protected:
    // 鼠标按下
    void mousePressEvent(QMouseEvent* ev);
    // 鼠标释放
    void mouseReleaseEvent(QMouseEvent* ev);
    // 鼠标悬停
    void enterEvent(QEvent* ev);
    // 鼠标离开悬停
    void leaveEvent(QEvent* ev);
    // 绘图
    void paintEvent(QPaintEvent* ev);

signals:
};

#endif // MYBUTTON_H

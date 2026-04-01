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

        // DEBUG
        // bool loadOk = m_pixmap.load(m_normal);
        // if (!loadOk) qDebug() << "[MyButton] 普通状态图片加载失败：" << m_normal;

        update();
    }

protected:
    // 鼠标按下
    void mousePressEvent(QMouseEvent* ev) override;
    // 鼠标释放
    void mouseReleaseEvent(QMouseEvent* ev) override;
    // 鼠标悬停
    void enterEvent(QEnterEvent* ev) override;
    // 鼠标离开悬停
    void leaveEvent(QEvent* ev) override;
    // 绘图
    void paintEvent(QPaintEvent* ev) override;

signals:
};

#endif // MYBUTTON_H

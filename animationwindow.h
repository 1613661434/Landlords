#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>

class AnimationWindow : public QWidget
{
    Q_OBJECT
public:
    enum class Type : char
    {
        Sequence,
        Pair
    };

private:
    QPixmap m_image;
    int m_index = 0;
    int m_x = 0;

public:
    explicit AnimationWindow(QWidget* parent = nullptr);

    // 显示下注分数
    void showBetScore(int point);
    // 显示顺子和连对
    void showSequence(Type type);
    // 显示王炸
    void showJokerBomb();
    // 显示炸弹
    void showBomb();
    // 显示飞机
    void showPlane();

protected:
    void paintEvent(QPaintEvent* ev) override;

signals:
};

#endif // ANIMATIONWINDOW_H

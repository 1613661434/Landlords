#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui
{
    class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    enum Panel : char
    {
        Start,
        PlayCard,
        PassOrPlay,
        CallLord,
        Empty
    };

private:
    Ui::ButtonGroup* ui;

public:
    explicit ButtonGroup(QWidget* parent = nullptr);
    ~ButtonGroup();

    // 初始化按钮
    void initButtons();

    // 处理Page页的切换（point只有type为CallLord才有用）
    void selectPanel(ButtonGroup::Panel type, int point = 0);

signals:
    // 开始游戏
    void startGame();
    // 出牌
    void playHand();
    // 不出牌
    void pass();
    // 抢地主
    void betPoint(int point);
};

#endif // BUTTONGROUP_H

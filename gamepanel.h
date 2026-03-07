#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include <QPainter>
#include "gamecontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT
private:
    QPixmap m_bkImage;
    GameControl* m_gameCtl;
    QVector<Player*> m_playerList; // 顺序：左侧机器人，右侧机器人，当前玩家

public:
    GamePanel(QWidget* parent = nullptr);
    ~GamePanel();

    // 初始化游戏控制类信息
    void gameControlInit();
    // 更新分数面板的分数
    void updatePlayerScore();

protected:
    // 绘图
    void paintEvent(QPaintEvent* ev) override;

private:
    Ui::GamePanel* ui;
};
#endif // GAMEPANEL_H

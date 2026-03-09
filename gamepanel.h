#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include "gamecontrol.h"
#include "cardpanel.h"

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
    enum CardAlign : char
    {
        Horizontal,
        Vertical
    };
    struct PlayerContext
    {
        // 1. 玩家扑克牌显示的区域
        QRect cardRect;
        // 2. 出牌的区域
        QRect playHandRect;
        // 3. 扑克牌的对齐方式(水平 or 垂直)
        CardAlign align;
        // 4. 扑克牌显示正面还是背面
        bool isFrontSide;
        // 5. 游戏过程中的提示信息, 比如: 不出
        QLabel* info;
        // 6. 玩家的头像
        QLabel* roleImg;
        // 7. 玩家刚打出的牌
        Cards lastCards;
    };

private:
    Ui::GamePanel* ui;
    QPixmap m_bkImage;
    GameControl* m_gameCtl;
    QVector<Player*> m_playerList; // 顺序：左侧机器人，右侧机器人，当前玩家
    QMap<Card, CardPanel*> m_cardMap;
    QSize m_cardSize;
    QPixmap m_cardBackImg;
    QMap<Player*, PlayerContext> m_contextMap;
    CardPanel* m_baseCard;
    CardPanel* m_moveCard;
    QVector<CardPanel*> m_last3Card;
    QPoint m_baseCardPos;
    GameControl::GameStatus m_gameStatus;
    QTimer* m_timer;

public:
    GamePanel(QWidget* parent = nullptr);
    ~GamePanel();

    // 初始化游戏控制类信息
    void gameControlInit();
    // 更新分数面板的分数
    void updatePlayerScore();
    // 切割并存储图片
    void initCardMap();
    // 裁剪卡牌图片
    void cropImage(const QPixmap& pixmap, int x, int y, const Card& card);
    // 初始化游戏按钮组
    void initButtonsGroup();
    // 初始化玩家在窗口中的上下文环境
    void initPlayerContext();
    // 初始化游戏场景
    void initGameScene();
    // 处理游戏的状态
    void gameStatusPrecess(GameControl::GameStatus status);
    // 发牌
    void startDispatchCard();
    // 移动扑克牌
    void cardMoveStep(Player* player, int curPos);

    // 定时器的处理动作
    void onDispatchCard();

protected:
    // 绘图
    void paintEvent(QPaintEvent* ev) override;
};
#endif // GAMEPANEL_H

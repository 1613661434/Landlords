#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "userplayer.h"
#include "cards.h"

class GameControl : public QObject
{
    Q_OBJECT
public:
    // 游戏状态
    enum GameStatus : char
    {
        DispatchCard, // 发牌
        CallingLord,  // 叫地主
        PlayingHand   // 出牌
    };
    // 玩家状态
    enum PlayerStatus : char
    {
        ThinkingForCallLord, // 考虑叫地主
        ThinkingForPlayHand, // 考虑出牌
        Winning              // 获胜
    };

private:
    Robot* m_robotLeft;
    Robot* m_robotRight;
    UserPlayer* m_user;
    Player* m_currPlayer;
    Player* m_pendPlayer;
    Cards m_pendCards;
    Cards m_allCards;

public:
    explicit GameControl(QObject* parent = nullptr);

    // 初始化玩家
    void playerInit();

    // 初始化扑克牌
    void initAllCards();

    // 每次发一张牌
    inline Card takeOneCard() { return m_allCards.takeRandCard(); }

    // 得到最后的三张底牌
    inline Cards getSurplusCards() { return m_allCards; }

    // 重置卡牌数据
    void resetCardData();

    // 准备叫地主
    inline void startLordCard()
    {
        m_currPlayer->prepareCallLord();
        emit playerStatusChanged(m_currPlayer, GameControl::PlayerStatus::ThinkingForCallLord);
    }

    // 成为地主
    void becomeLord(Player* player);

    // 清空所有玩家的得分
    void clearPlayerScore();

    // 处理叫地主

    // 处理出牌

    // Get&Set
    inline Robot* getLeftRobot() const { return m_robotLeft; }
    inline Robot* getRightRobot() const { return m_robotRight; }
    inline UserPlayer* getUserPlayer() const { return m_user; }

    inline void setCurrentPlayer(Player* player) { m_currPlayer = player; }
    inline Player* getCurrentPlayer() const { return m_currPlayer; }

    inline Player* getPendPlayer() const { return m_pendPlayer; }
    inline Cards getPendCards() const { return m_pendCards; }
signals:
    void playerStatusChanged(Player* player, GameControl::PlayerStatus status);
};

#endif // GAMECONTROL_H

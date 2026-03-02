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

public:
    explicit GameControl(QObject* parent = nullptr);

    // 初始化玩家
    void playerInit();

    // 初始化扑克牌

    // Get&Set
    inline Robot* getLeftRobot() const { return m_robotLeft; }
    inline Robot* getRightRobot() const { return m_robotRight; }
    inline UserPlayer* getUserPlayer() const { return m_user; }

    inline void setCurrentPlayer(Player* player) { m_currPlayer = player; }
    inline Player* getCurrentPlayer() const { return m_currPlayer; }

    inline Player* getPendPlayer() const { return m_pendPlayer; }
    inline Cards getPendCards() const { return m_pendCards; }
signals:
};

#endif // GAMECONTROL_H

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
    enum class GameStatus : char
    {
        DispatchCard, // 发牌
        CallingLord,  // 叫地主
        PlayingHand   // 出牌
    };
    // 玩家状态
    enum class PlayerStatus : char
    {
        ThinkingForCallLord, // 考虑叫地主
        ThinkingForPlayHand, // 考虑出牌
        Winning              // 获胜
    };
    // 叫地主保存的数据
    struct BetRecord
    {
        Player* player;
        int point;
        int times; // 第几次叫地主

        BetRecord() { reset(); }
        void reset()
        {
            player = nullptr;
            point = 0;
            times = 0;
        }
    };

private:
    Robot* m_robotLeft;
    Robot* m_robotRight;
    UserPlayer* m_user;
    Player* m_currPlayer;
    Player* m_pendPlayer;
    Cards m_pendCards;
    Cards m_allCards;
    BetRecord m_betRecord;

public:
    explicit GameControl(QObject* parent = nullptr);

    // 初始化玩家
    void playerInit();

    // 初始化扑克牌
    void initAllCards();

    // 每次发一张牌
    inline Card takeOneCard() { return m_allCards.takeRandCard(); }

    // 得到最后的三张底牌
    inline Cards getSurplusCards() const { return m_allCards; }

    // 重置卡牌数据
    void resetCardData();

    // 准备叫地主
    inline void startLordCard()
    {
        m_currPlayer->prepareCallLord();
        emit playerStatusChanged(m_currPlayer, PlayerStatus::ThinkingForCallLord);
    }

    // 成为地主
    void becomeLord(Player* player);

    // 清空所有玩家的得分
    void clearPlayerScore();

    // 得到玩家下注的最高分数
    inline int getPlayerMaxBet() const { return m_betRecord.point; }

    // 处理叫地主
    void onGrabBet(Player* player, int point);

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
    // 玩家状态变化
    void playerStatusChanged(Player* player, PlayerStatus status);
    // 通知玩家抢地主了
    void notifyGrabLordBet(Player* player, int point, bool isFirst);
    // 游戏状态变化
    void gameStatusChanged(GameStatus status);
};

#endif // GAMECONTROL_H

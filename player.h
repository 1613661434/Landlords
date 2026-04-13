#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMutex>
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
    // == 枚举定义 ==
public:
    enum class Role : char
    {
        Lord,
        Farmer
    }; // 角色

    enum class Sex : char
    {
        Man,
        Woman
    }; // 性别

    enum class Direction : char
    {
        Left,
        Right
    }; // 头像的显示方位

    enum class Type : char
    {
        Robot,
        User,
        Unknown
    }; // 玩家的类型

    // == 成员变量 ==
protected:
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_direction;
    Type m_type;
    int m_score = 0;
    bool m_isWin = false;
    Player* m_prev = nullptr;
    Player* m_next = nullptr;
    Cards m_cards;
    Cards m_pendCards;
    Player* m_pendPlayer = nullptr;
    mutable QMutex m_cardMutex;
    mutable QMutex m_roleMutex;

    // == 函数 ==
public:
    explicit Player(QObject* parent = nullptr);
    explicit Player(QString name, QObject* parent = nullptr);

    // 名字
    inline void setName(QString name) { m_name = name; }
    inline QString getName() const { return m_name; }

    // 角色
    void setRole(Role role);
    Role getRole() const;

    // 性别
    inline void setSex(Sex sex) { m_sex = sex; }
    inline Sex getSex() const { return m_sex; }

    // 头像的显示方位
    inline void setDirection(Direction direction) { m_direction = direction; }
    inline Direction getDirection() const { return m_direction; }

    // 玩家的类型
    inline void setType(Type type) { m_type = type; }
    inline Type getType() const { return m_type; }

    // 分数
    inline void setScore(int score) { m_score = score; }
    inline int getScore() const { return m_score; }

    // 游戏结果
    inline void setIsWin(bool flag) { m_isWin = flag; }
    inline bool getIsWin() const { return m_isWin; }

    // 游戏上家
    inline void setPrevPlayer(Player* prev) { m_prev = prev; }
    inline Player* getPrevPlayer() const { return m_prev; }

    // 游戏下家
    inline void setNextPlayer(Player* next) { m_next = next; }
    inline Player* getNextPlayer() const { return m_next; }

    // 抢地主
    inline void grabLordBet(int point) { emit notifyGrabLordBet(this, point); };

    // 存储扑克牌（发牌阶段）
    void storeDispatchCard(const Card& card);
    void storeDispatchCard(const Cards& cards);

    // 存储出牌玩家对象和打出的牌
    inline void storePendingInfo(Player* player, const Cards& cards)
    {
        m_pendPlayer = player;
        m_pendCards = cards;
    }

    Cards getCards() const;      // 得到所有牌
    void clearCards();           // 清空所有牌
    void playHand(Cards& cards); // 出牌

    // 待处理扑克牌相关
    inline void setPendingInfo(Player* player, const Cards& cards)
    {
        m_pendPlayer = player;
        m_pendCards = cards;
    }
    inline Player* getPendPlayer() { return m_pendPlayer; }
    inline Cards getPendCards() { return m_pendCards; }

    // == 虚函数 ==
public:
    virtual void prepareCallLord() = 0; // 准备叫地主
    virtual void preparePlayHand() = 0; // 准备出牌
    virtual void thinkCallLord() = 0;   // 考虑叫地主
    virtual void thinkPlayHand() = 0;   // 考虑出牌

    // == 信号 ==
signals:
    // 通知已经叫地主下注
    void notifyGrabLordBet(Player* player, int point);
    // 通知已经出牌
    void notifyPlayHand(Player* player, Cards& cards);
    // 通知已经发牌了
    void notifyPickCards(Player* player, const Cards& cards);
};

#endif // PLAYER_H

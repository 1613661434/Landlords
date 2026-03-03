#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"
#include "ol_enum_char_ops.h"

class Player : public QObject
{
    Q_OBJECT
    // == 枚举定义 ==
public:
    enum class Role : char
    {
        Load,
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
    int m_score;
    bool m_isWin;
    Player* m_prev;
    Player* m_next;
    Cards m_cards;
    Cards m_pendCards;
    Player* m_pendPlayer;

    // == 函数 ==
public:
    explicit Player(QObject* parent = nullptr);
    explicit Player(QString name, QObject* parent = nullptr);

    // 名字
    inline void setName(QString name) { m_name = name; }
    inline QString getName() const { return m_name; }

    // 角色
    inline void setRole(Role role) { m_role = role; }
    inline Role getRole() const { return m_role; }

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
    inline void grabLordBet(int point) { /*WILL DO*/ };

    // 存储扑克牌（发牌阶段）
    inline void storeDispatchCard(Card& card) { m_cards.add(card); }
    inline void storeDispatchCard(Cards& cards) { m_cards.add(cards); }

    inline Cards getCards() const { return m_cards; }             // 得到所有牌
    inline void clearCards() { m_cards.clear(); }                 // 清空所有牌
    inline void playHand(Cards& cards) { m_cards.remove(cards); } // 出牌

    // 待处理扑克牌相关
    inline void setPendingInfo(Player* player, Cards& cards)
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
signals:
};

#endif // PLAYER_H

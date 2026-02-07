#ifndef CARD_H
#define CARD_H

#include <QVector>

class Card
{
    // == 枚举定义 ==
public:
    // 花色
    enum class CardSuit : char
    {
        Suit_Begin,
        Diamond, // 方块
        Club,    // 梅花
        Heart,   // 红心
        Spade,   // 黑桃
        Suit_End
    };

    // 点数
    enum class CardPoint : char
    {
        Card_Begin,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ, // Small Joker
        Card_Bj, // Big Joker
        Card_End
    };

    // == 成员变量 ==
private:
    CardSuit m_suit;
    CardPoint m_point;

    // == 函数 ==
public:
    Card();

    inline static bool lessSort(const Card& a, const Card& b)
    {
        if (a.m_point == b.m_point) return a.m_suit < b.m_suit;
        return a.m_point < b.m_point;
    }
    inline static bool greaterSort(const Card& a, const Card& b)
    {
        if (a.m_point == b.m_point) return a.m_suit > b.m_suit;
        return a.m_point > b.m_point;
    }

    inline void setCardSuit(CardSuit suit) { m_suit = suit; }
    inline void setCardPoint(CardPoint point) { m_point = point; }
    inline CardSuit getCardSuit() const { return m_suit; }
    inline CardPoint getCardPoint() const { return m_point; }

    // == 友元函数 ==
    friend inline bool operator==(const Card& a, const Card& b) { return (a.m_suit == b.m_suit) && (a.m_point == b.m_point); }
    friend inline uint qHash(const Card& card, uint seed = 0) { return qHash(static_cast<uint>(card.m_suit), qHash(static_cast<uint>(card.m_point), seed)); }
};

using CardList = QVector<Card>;

#endif // CARD_H

#ifndef CARD_H
#define CARD_H

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

    inline void setCardSuit(CardSuit suit) { m_suit = suit; }
    inline void setCardPoint(CardPoint point) { m_point = point; }
    inline CardSuit getCardSuit() const { return m_suit; }
    inline CardPoint getCardPoint() const { return m_point; }
};

#endif // CARD_H

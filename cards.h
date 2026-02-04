#ifndef CARDS_H
#define CARDS_H

#include "card.h"
#include <QSet>

class Cards
{
private:
    QSet<Card> m_cards;

public:
    Cards();

    // 添加扑克牌
    inline void add(Card& card) { m_cards.insert(card); }
    inline void add(Cards& cards) { m_cards.unite(cards.m_cards); }

    Cards& operator<<(const Card& card)
    {
        m_cards.insert(card);
        return *this;
    }
    Cards& operator<<(const Cards& cards)
    {
        m_cards.unite(cards.m_cards);
        return *this;
    }

    // 删除扑克牌
    inline void remove(Card& card) { m_cards.remove(card); }
    inline void remove(Cards& cards) { m_cards.subtract(cards.m_cards); }

    inline int cardCount() { return m_cards.size(); }
    inline bool isEmpty() { return m_cards.isEmpty(); }
    inline void clear() { m_cards.clear(); }

    Card::CardPoint maxPoint();                                                   // 最大点数
    Card::CardPoint minPoint();                                                   // 最小点数
    int pointCount(Card::CardPoint point);                                        // 指定点数牌的数量
    bool contains(const Card& card) { return m_cards.contains(card); }            // 某张牌是否在集合中
    bool contains(const Cards& cards) { return m_cards.contains(cards.m_cards); } // 某几张牌是否在集合中
};

#endif // CARDS_H

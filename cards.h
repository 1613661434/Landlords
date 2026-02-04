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
};

#endif // CARDS_H

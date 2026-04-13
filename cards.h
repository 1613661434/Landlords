#ifndef CARDS_H
#define CARDS_H

#include "card.h"
#include <QSet>
#include <QRandomGenerator>
#include <algorithm>

class Cards
{
public:
    enum class SortType : char
    {
        Asc,
        Desc,
        NoSort
    };

private:
    QSet<Card> m_cards;

public:
    Cards();
    explicit Cards(const Card& card);

    // 添加扑克牌
    inline void add(const Card& card) { m_cards.insert(card); }
    inline void add(const Cards& cards) { m_cards.unite(cards.m_cards); }
    inline void add(const QVector<Cards>& v_cards)
    {
        for (const auto& cards : v_cards) m_cards.unite(cards.m_cards);
    }

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
    inline void remove(const Card& card) { m_cards.remove(card); }
    inline void remove(const Cards& cards) { m_cards.subtract(cards.m_cards); }
    inline void remove(const QVector<Cards>& v_cards)
    {
        for (const auto& cards : v_cards) m_cards.subtract(cards.m_cards);
    }

    inline int cardCount() const { return m_cards.size(); }
    inline bool isEmpty() const { return m_cards.isEmpty(); }
    inline void clear() { m_cards.clear(); }

    Card::CardPoint maxPoint() const;                                                          // 最大点数
    Card::CardPoint minPoint() const;                                                          // 最小点数
    int pointCount(Card::CardPoint point) const;                                               // 指定点数牌的数量
    inline bool contains(const Card& card) const { return m_cards.contains(card); }            // 某张牌是否在集合中
    inline bool contains(const Cards& cards) const { return m_cards.contains(cards.m_cards); } // 某几张牌是否在集合中
    Card takeRandomCard();                                                                     // 随机取一张牌
    CardList toCardList(SortType type = SortType::Desc) const;
};

#endif // CARDS_H

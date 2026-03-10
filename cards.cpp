#include "cards.h"

Cards::Cards() {}

Cards::Cards(const Card& card)
{
    m_cards.insert(card);
}

Card::CardPoint Cards::maxPoint() const
{
    Card::CardPoint maxPt = Card::CardPoint::Card_Begin;

    if (m_cards.isEmpty()) return maxPt;

    for (const auto& e : m_cards)
    {
        if (e.getCardPoint() > maxPt) maxPt = e.getCardPoint();
    }

    return maxPt;
}

Card::CardPoint Cards::minPoint() const
{
    Card::CardPoint minPt = Card::CardPoint::Card_End;

    if (m_cards.isEmpty()) return minPt;

    for (const auto& e : m_cards)
    {
        if (e.getCardPoint() < minPt) minPt = e.getCardPoint();
    }

    return minPt;
}

int Cards::pointCount(Card::CardPoint point) const
{
    int count = 0;

    if (m_cards.isEmpty()) return count;

    for (const auto& e : m_cards)
    {
        if (e.getCardPoint() == point) ++count;
    }

    return count;
}

Card Cards::takeRandCard()
{
    int num = QRandomGenerator::global()->bounded(m_cards.size());
    QSet<Card>::const_iterator it = m_cards.constBegin();
    while (num > 0)
    {
        --num;
        ++it;
    }
    Card card = *it;
    m_cards.erase(it);
    return card;
}

CardList Cards::toCardList(SortType type) const
{
    CardList list(m_cards.begin(), m_cards.end());

    switch (type)
    {
    case SortType::Asc:
        std::sort(list.begin(), list.end(), Card::lessSort);
        break;
    case SortType::Desc:
        std::sort(list.begin(), list.end(), Card::greaterSort);
        break;
    case SortType::NoSort:
        break;
    }

    return list;
}

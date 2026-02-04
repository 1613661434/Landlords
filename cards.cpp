#include "cards.h"

Cards::Cards() {}

Card::CardPoint Cards::maxPoint()
{
    Card::CardPoint maxPt = Card::CardPoint::Card_Begin;

    if (m_cards.isEmpty()) return maxPt;

    for (const auto& e : m_cards)
    {
        if (e.getCardPoint() > maxPt) maxPt = e.getCardPoint();
    }

    return maxPt;
}

Card::CardPoint Cards::minPoint()
{
    Card::CardPoint minPt = Card::CardPoint::Card_End;

    if (m_cards.isEmpty()) return minPt;

    for (const auto& e : m_cards)
    {
        if (e.getCardPoint() < minPt) minPt = e.getCardPoint();
    }

    return minPt;
}

int Cards::pointCount(Card::CardPoint point)
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

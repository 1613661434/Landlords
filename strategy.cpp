#include "strategy.h"

Strategy::Strategy(Player* player, const Cards& cards) : m_player(player), m_cards(cards) {}

Cards Strategy::findSamePointCards(Card::CardPoint point, int count) const
{
    Cards retCards = Cards();

    if (count < 1 || count > 4) return retCards;

    // 大小王
    if (point == Card::CardPoint::Card_SJ || point == Card::CardPoint::Card_BJ)
    {
        if (count > 1) return retCards;

        Card card = Card(point, Card::CardSuit::Suit_Begin);
        if (m_cards.contains(card)) retCards.add(card);
        return retCards;
    }

    // 不是大小王
    int findCount = 0;
    Card card;
    card.setCardPoint(point);
    for (Card::CardSuit suit = Card::CardSuit::Suit_Begin + 1; suit < Card::CardSuit::Suit_End; ++suit)
    {
        card.setCardSuit(suit);
        if (m_cards.contains(card))
        {
            ++findCount;
            retCards.add(card);
            if (findCount == count) return retCards;
        }
    }

    retCards.clear();
    return retCards;
}

QVector<Cards> Strategy::findCardsByCount(int count) const
{
    QVector<Cards> cardsArray;

    if (count < 1 || count > 4) return cardsArray;

    for (Card::CardPoint point = Card::CardPoint::Card_Begin + 1; point < Card::CardPoint::Card_End; ++point)
    {
        if (m_cards.pointCount(point) == count) cardsArray << findSamePointCards(point, count);
    }

    return cardsArray;
}

Cards Strategy::getRangeCards(Card::CardPoint begin, Card::CardPoint end) const
{
    Cards rangeCards;

    for (; begin < end; ++begin)
    {
        rangeCards << findSamePointCards(begin, m_cards.pointCount(begin));
    }

    return rangeCards;
}
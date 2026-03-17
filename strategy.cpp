#include "strategy.h"

Strategy::Strategy(Player* player, const Cards& cards) : m_player(player), m_cards(cards) {}

Cards Strategy::findSamePointCards(Card::CardPoint point, int count)
{
    Cards retCards = Cards();

    if (count < 1 || count > 4)
    {
        return retCards;
    }

    // 大小王
    if (point == Card::CardPoint::Card_SJ || point == Card::CardPoint::Card_BJ)
    {
        if (count > 1)
        {
            return retCards;
        }

        Card card;
        card.setCardPoint(point);
        card.setCardSuit(Card::CardSuit::Suit_Begin);
        if (m_cards.contains(card))
        {
            retCards.add(card);
            return retCards;
        }
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
            if (findCount == count)
            {
                return retCards;
            }
        }
    }

    retCards.clear();
    return retCards;
}

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
        Cards cards = findSamePointCards(begin, m_cards.pointCount(begin));
        if (!cards.isEmpty()) rangeCards << cards;
    }

    return rangeCards;
}

QVector<Cards> Strategy::findCardType(PlayHand hand, bool beat) const
{
    PlayHand::HandType type = hand.getHandType();
    Card::CardPoint point = hand.getCardPoint();
    int extra = hand.getExtra();

    // 确定起始点数
    Card::CardPoint beginPoint = beat ? Card::CardPoint(point + 1) : Card::CardPoint::Card_3;

    switch (type)
    {
    case PlayHand::Hand_Single:
        return findCardsByCountFromPoint(beginPoint, 1); // BUG：按道理应该相同点数但是花色大的也加入，先这样子
    case PlayHand::Hand_Pair:
        return findCardsByCountFromPoint(beginPoint, 2);
    case PlayHand::Hand_Triple:
        return findCardsByCountFromPoint(beginPoint, 3);
    case PlayHand::Hand_Triple_Single:
    case PlayHand::Hand_Triple_Pair:
    case PlayHand::Hand_Plane:
    case PlayHand::Hand_Plane_Two_Single:
    case PlayHand::Hand_Plane_Two_Pair:
    case PlayHand::Hand_Seq_Pair:
    case PlayHand::Hand_Seq_Single:
    case PlayHand::Hand_Bomb:
    default:
        return QVector<Cards>();
    }
}

QVector<Cards> Strategy::findCardsByCountFromPoint(Card::CardPoint point, int number) const
{
    QVector<Cards> findCardsArray;

    if (number < 1 || number > 4) return findCardsArray;

    for (; point < Card::CardPoint::Card_End; ++point)
    {
        // 目的是尽量不拆分别的牌型
        if (m_cards.pointCount(point) == number) findCardsArray << findSamePointCards(point, number);
    }

    return findCardsArray;
}

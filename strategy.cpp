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
        return getCardsByCountFromPoint(beginPoint, 1); // BUG：按道理应该相同点数但是花色大的也加入，先这样子
    case PlayHand::Hand_Pair:
        return getCardsByCountFromPoint(beginPoint, 2);
    case PlayHand::Hand_Triple:
        return getCardsByCountFromPoint(beginPoint, 3);
    case PlayHand::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint, PlayHand::HandType::Hand_Single);
    case PlayHand::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint, PlayHand::HandType::Hand_Pair);
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

QVector<Cards> Strategy::getCardsByCountFromPoint(Card::CardPoint point, int number) const
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

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type) const
{
    if (type != PlayHand::HandType::Hand_Single && type != PlayHand::HandType::Hand_Pair) return QVector<Cards>();

    // 找到点数相同的三张牌
    QVector<Cards> findCardArray = getCardsByCountFromPoint(begin, 3);
    if (findCardArray.isEmpty()) return findCardArray;

    // 将找到的牌从用户手中删除
    Cards remainCards = m_cards;
    remainCards.remove(findCardArray);

    // 搜索牌型(单牌或者成对的牌)
    Strategy st(m_player, remainCards);
    QVector<Cards> cardsArray = st.findCardType(PlayHand(type, Card::CardPoint::Card_Begin), false);
    if (cardsArray.isEmpty()) return cardsArray;

    // 将找到的牌和三张点数相同的牌进行组合
    switch (type)
    {
    case PlayHand::HandType::Hand_Pair:
        findCardArray[1].add(cardsArray.at(1));
    case PlayHand::HandType::Hand_Single:
        findCardArray[0].add(cardsArray.at(0));
    }

    // 将最终结果返回给函数调用者
    return findCardArray;
}

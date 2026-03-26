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
        return getPlane(beginPoint);
    case PlayHand::Hand_Plane_Two_Single:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::HandType::Hand_Single);
    case PlayHand::Hand_Plane_Two_Pair:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::HandType::Hand_Pair);
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
    for (int i = 0, size = (int)findCardArray.size(); i < size; ++i) findCardArray[i].add(cardsArray.at(0));

    // 将最终结果返回给函数调用者
    return findCardArray;
}

QVector<Cards> Strategy::getPlane(Card::CardPoint begin) const
{
    QVector<Cards> findCardArray;
    for (; begin < Card::CardPoint::Card_A; ++begin)
    {
        // 根据点数和数量进行搜索
        Cards prevCards = findSamePointCards(begin, 3);
        Cards nextCards = findSamePointCards(begin + 1, 3);
        if (!prevCards.isEmpty() && !nextCards.isEmpty())
        {
            prevCards << nextCards;
            findCardArray << prevCards;
        }
    }
    return findCardArray;
}

QVector<Cards> Strategy::getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type) const
{
    if (type != PlayHand::HandType::Hand_Single && type != PlayHand::HandType::Hand_Pair) return QVector<Cards>();

    // 找到点数相同的三张牌
    QVector<Cards> findCardArray = getPlane(begin);
    if (findCardArray.isEmpty()) return findCardArray;

    // 将找到的牌从用户手中删除
    Cards remainCards = m_cards;
    remainCards.remove(findCardArray);

    // 搜索牌型(单牌或者成对的牌)
    Strategy st(m_player, remainCards);
    QVector<Cards> cardsArray = st.findCardType(PlayHand(type, Card::CardPoint::Card_Begin), false);
    if (cardsArray.size() < 2)
    {
        cardsArray.clear();
        return cardsArray;
    }

    // 找到了, 将其添加到飞机组合中
    cardsArray[0] << cardsArray[1];
    for (int i = 0, size = (int)findCardArray.size(); i < size; ++i) findCardArray[i].add(cardsArray.at(0));

    // 将最终结果返回给函数调用者
    return findCardArray;
}

QVector<Cards> Strategy::getSepPairOrSeqSingle(Card::CardPoint begin, int extra, bool beat) const
{
    QVector<Cards> findCardsArray;
    if (beat)
    {
        // 最少3个, 最大A
        for (Card::CardPoint point = begin; point <= Card::CardPoint::Card_Q; ++point)
        {
            bool found = true;
            Cards seqCards;
            for (int i = 0; i < extra; ++i)
            {
                // 基于点数和数量进行牌的搜索
                Cards cards = findSamePointCards(point + i, 2);
                if (cards.isEmpty() || (point + extra >= Card::CardPoint::Card_2))
                {
                    found = false;
                    seqCards.clear();
                    break;
                }
                seqCards << cards;
            }
            if (found)
            {
                findCardsArray << seqCards;
                return findCardsArray;
            }
        }
    }
    else
    {
        for (Card::CardPoint point = begin; point <= Card::CardPoint::Card_Q; ++point)
        {
            // 找到三个点数连续的对
            Cards cards0 = findSamePointCards(point, 2);
            Cards cards1 = findSamePointCards(point + 1, 2);
            Cards cards2 = findSamePointCards(point + 2, 2);
            if (cards0.isEmpty() || cards1.isEmpty() || cards2.isEmpty()) continue;

            // 将找到的这个基础连对存储起来
            Cards baseSeq;
            baseSeq << cards0 << cards1 << cards2;
            // 连对存储到容器中
            findCardsArray << baseSeq;

            int followed = 3;
            Cards alreadyFollowedCards; // 存储后续找到的满足条件的连对

            while (true)
            {
                // 新的起始点数
                Card::CardPoint followedPoint = Card::CardPoint(point + followed);
                // 判断是否超出了上限
                if (followedPoint >= Card::CardPoint::Card_2) break;

                Cards follwedCards = findSamePointCards(followedPoint, 2);
                if (follwedCards.isEmpty()) break;

                alreadyFollowedCards << follwedCards;
                Cards newSeq = baseSeq;
                newSeq << alreadyFollowedCards;
                findCardsArray << newSeq;
                ++followed;
            }
        }
    }
    return findCardsArray;
}

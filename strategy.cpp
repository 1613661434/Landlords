#include "strategy.h"
#include <functional>

Strategy::Strategy(Player* player, const Cards& cards) : m_player(player), m_cards(cards) {}

Cards Strategy::makeStrategy() const
{
    // 得到出牌玩家对象以及打出的牌
    Player* pendPlayer = m_player->getPendPlayer();
    Cards pendCards = m_player->getPendCards();

    // 判断上次出牌的玩家是不是我自己
    if (pendPlayer == m_player || pendPlayer == nullptr)
    {
        // 直接出牌
        // 如果是我自己, 出牌没有限制
        return firstPlay();
    }

    // 如果不是我自己需要找比出牌玩家点数大的牌
    Cards beatCards = getGreaterCards(PlayHand(pendCards));
    // 找到了点数大的牌需要考虑是否出牌
    if (whetherToBeat(beatCards))
    {
        return beatCards;
    }

    return Cards();
}

Cards Strategy::firstPlay() const
{
    return Cards();
}

Cards Strategy::getGreaterCards(PlayHand type) const
{
    return Cards();
}

bool Strategy::whetherToBeat(const Cards& cards) const
{
    // 没有找到能够击败对方的牌
    if (cards.isEmpty()) return false;

    // 得到出牌玩家的对象
    Player* pendPlayer = m_player->getPendPlayer();
    if (m_player->getRole() == pendPlayer->getRole())
    {
        // 手里的牌所剩无几并且是一个完整的牌型
        Cards left = m_cards;
        left.remove(cards);
        if (PlayHand(left).getHandType() != PlayHand::HandType::Hand_Unknown) return true;

        // 如果cards对象中的牌的最小点数是2,大小王 --> 不出牌
        Card::CardPoint basePoint = PlayHand(cards).getCardPoint();
        if (basePoint == Card::CardPoint::Card_2 || basePoint == Card::CardPoint::Card_SJ || basePoint == Card::CardPoint::Card_BJ) return false;
    }
    else
    {
        PlayHand myHand(cards);
        // 如果是三个2带一,或者带一对, 不出牌(保存实力)
        if ((myHand.getHandType() == PlayHand::HandType::Hand_Triple_Single || myHand.getHandType() == PlayHand::HandType::Hand_Triple_Pair) &&
            myHand.getCardPoint() == Card::CardPoint::Card_2) return false;

        // 如果cards是对2, 并且出牌玩家手中的牌数量大于等于10 && 自己的牌的数量大于等于5, 暂时放弃出牌
        if (myHand.getHandType() == PlayHand::HandType::Hand_Pair && myHand.getCardPoint() == Card::CardPoint::Card_2 &&
            pendPlayer->getCards().cardCount() >= 10 && m_player->getCards().cardCount() >= 5) return false;
    }

    return true;
}

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

QVector<Cards> Strategy::findCardType(PlayHand hand, bool isBeat) const
{
    PlayHand::HandType type = hand.getHandType();
    Card::CardPoint point = hand.getCardPoint();
    int extra = hand.getExtra();

    // 确定起始点数
    Card::CardPoint beginPoint = isBeat ? Card::CardPoint(point + 1) : Card::CardPoint::Card_3;

    switch (type)
    {
    case PlayHand::Hand_Single:
        return getCardsByCountFromPoint(beginPoint, 1); // BUG：按道理应该相同点数但是花色大的也加入，先这样子
    case PlayHand::Hand_Pair:
        return getCardsByCountFromPoint(beginPoint, 2);
    case PlayHand::Hand_Triple:
        return getCardsByCountFromPoint(beginPoint, 3);
    case PlayHand::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint, false);
    case PlayHand::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint, true);
    case PlayHand::Hand_Plane:
        return getPlane(beginPoint);
    case PlayHand::Hand_Plane_Two_Single:
        return getPlane2SingleOr2Pair(beginPoint, false);
    case PlayHand::Hand_Plane_Two_Pair:
        return getPlane2SingleOr2Pair(beginPoint, true);
    case PlayHand::Hand_Seq_Single:
        return getSeqSingleOrSepPair(beginPoint, extra, isBeat, false);
    case PlayHand::Hand_Seq_Pair:
        return getSeqSingleOrSepPair(beginPoint, extra, isBeat, true);
    case PlayHand::Hand_Bomb:
        return getBomb(beginPoint);
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

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, bool isPair) const
{
    PlayHand::HandType type = isPair ? PlayHand::HandType::Hand_Pair : PlayHand::HandType::Hand_Single;

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

QVector<Cards> Strategy::getPlane2SingleOr2Pair(Card::CardPoint begin, bool isPair) const
{
    PlayHand::HandType type = isPair ? PlayHand::HandType::Hand_Pair : PlayHand::HandType::Hand_Single;

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

QVector<Cards> Strategy::getSeqSingleOrSepPair(Card::CardPoint begin, int extra, bool isBeat, bool isPair) const
{
    QVector<Cards> findCardsArray;
    Card::CardPoint end;
    int number;
    int baseFollowed;
    std::function<Cards(Card::CardPoint)> getBaseSeqSingleOrPair;

    if (isPair)
    {
        end = Card::CardPoint::Card_K;
        number = 2;
        baseFollowed = 3;
    }
    else
    {
        end = Card::CardPoint::Card_J;
        number = 1;
        baseFollowed = 5;
    }

    getBaseSeqSingleOrPair = [this, number, baseFollowed](Card::CardPoint point)
    {
        Cards baseSeq;
        for (int i = 0; i < baseFollowed; ++i)
        {
            Cards cards = findSamePointCards(point + i, number);
            if (cards.isEmpty()) return Cards();
            baseSeq << cards;
        }
        return baseSeq;
    };

    if (isBeat)
    {
        // 最少3||5个, 最大A
        for (; begin < end; ++begin)
        {
            bool found = true;
            Cards seqCards;
            for (int i = 0; i < extra; ++i)
            {
                // 基于点数和数量进行牌的搜索
                Cards cards = findSamePointCards(begin + i, number);
                if (cards.isEmpty() || (begin + extra >= Card::CardPoint::Card_2))
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
        for (; begin < end; ++begin)
        {
            // 将找到的这个基础连对存储起来
            Cards baseSeq = getBaseSeqSingleOrPair(begin);
            if (baseSeq.isEmpty()) continue;

            // 连对存储到容器中
            findCardsArray << baseSeq;

            int followed = baseFollowed;
            Cards alreadyFollowedCards; // 存储后续找到的满足条件的连对

            while (true)
            {
                // 新的起始点数
                Card::CardPoint followedPoint = Card::CardPoint(begin + followed);
                // 判断是否超出了上限
                if (followedPoint >= Card::CardPoint::Card_2) break;

                Cards follwedCards = findSamePointCards(followedPoint, number);
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

QVector<Cards> Strategy::getBomb(Card::CardPoint begin) const
{
    QVector<Cards> findcardsArray;
    for (; begin < Card::CardPoint::Card_End; ++begin)
    {
        Cards cards = findSamePointCards(begin, 4);
        if (!cards.isEmpty()) findcardsArray << cards;
    }
    return findcardsArray;
}

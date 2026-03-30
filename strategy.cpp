#include "strategy.h"
#include <functional>
#include <QMap>

Strategy::Strategy(Player* player, const Cards& cards) : m_player(player), m_cards(cards) {}

Cards Strategy::makeStrategy() const
{
    // 得到出牌玩家对象以及打出的牌
    Player* pendPlayer = m_player->getPendPlayer();
    Cards pendCards = m_player->getPendCards();

    // 判断上次出牌的玩家是不是我自己(如果是我自己, 出牌没有限制)
    if (pendPlayer == m_player || pendPlayer == nullptr) return firstPlay();

    // 如果不是我自己需要找比出牌玩家点数大的牌
    Cards beatCards = getGreaterCards(PlayHand(pendCards));

    // 找到了点数大的牌需要考虑是否出牌
    if (whetherToBeat(beatCards)) return beatCards;

    return Cards();
}

Cards Strategy::firstPlay() const
{
    // 判断玩家手中是否只剩单一的牌型
    PlayHand hand(m_cards);
    if (hand.getHandType() != PlayHand::HandType::Hand_Unknown) return m_cards;

    // 不是单一牌型
    // 判断玩家手中是否有顺子
    QVector<Cards> optimalSeq = pickOptimalSeqSingles();
    if (!optimalSeq.isEmpty())
    {
        // 得到单牌的数量
        int baseNum = (int)findCardsByCount(1).size();
        // 把得到的顺子的集合从玩家手中删除
        Cards save = m_cards;
        save.remove(optimalSeq);
        int lastNum = (int)Strategy(m_player, save).findCardsByCount(1).size();
        if (lastNum < baseNum) return optimalSeq[0];
    }

    bool hasPlane = false, hasTriple = false, hasPair = false;
    Cards backup = m_cards;

    // 有没有炸弹
    QVector<Cards> bombArray = findCardType(PlayHand(PlayHand::HandType::Hand_Bomb, Card::CardPoint::Card_Begin), false);
    if (!bombArray.isEmpty()) backup.remove(bombArray);

    // 有没有飞机
    QVector<Cards> planeArray = Strategy(m_player, backup).findCardType(PlayHand(PlayHand::HandType::Hand_Plane, Card::CardPoint::Card_Begin), false);
    if (!planeArray.isEmpty())
    {
        hasPlane = true;
        backup.remove(planeArray);
    }

    // 有没有三张点数相同的牌
    QVector<Cards> seqTripleArray = Strategy(m_player, backup).findCardType(PlayHand(PlayHand::HandType::Hand_Triple, Card::CardPoint::Card_Begin), false);
    if (!seqTripleArray.isEmpty())
    {
        hasTriple = true;
        backup.remove(seqTripleArray);
    }

    // 有没有连对
    QVector<Cards> seqPairArray = Strategy(m_player, backup).findCardType(PlayHand(PlayHand::HandType::Hand_Seq_Pair, Card::CardPoint::Card_Begin), false);
    if (!seqPairArray.isEmpty())
    {
        hasPair = true;
        backup.remove(seqPairArray);
    }

    if (hasPair)
    {
        int maxIdx = 0, maxPair = seqPairArray[0].cardCount();
        for (int i = 1, size = (int)seqPairArray.size(); i < size; ++i)
        {
            if (seqPairArray[i].cardCount() > maxPair) maxIdx = i;
        }
        return seqPairArray[maxIdx];
    }

    return Cards();
}

Cards Strategy::getGreaterCards(PlayHand hand) const
{
    // 1. 出牌玩家和当前玩家不是一伙的
    Player* pendPlayer = m_player->getPendPlayer();
    if (pendPlayer != nullptr && pendPlayer->getRole() != m_player->getRole() && pendPlayer->getCards().cardCount() <= 3)
    {
        QVector<Cards> bombs = findCardsByCount(4);
        for (int i = 0, size = (int)bombs.size(); i < size; ++i)
        {
            if (PlayHand(bombs[i]).canBeat(hand)) return bombs[i];
        }
        // 搜索当前玩家手中有没有王炸
        Cards sj = findSamePointCards(Card::CardPoint::Card_SJ, 1);
        Cards bj = findSamePointCards(Card::CardPoint::Card_BJ, 1);
        if (!sj.isEmpty() && !bj.isEmpty())
        {
            sj << bj;
            return sj;
        }
    }

    // 2. 当前玩家和下一个玩家不是一伙的
    Player* nextPlayer = m_player->getNextPlayer();
    // 将玩家手中的顺子剔除出去
    Cards remain = m_cards;
    remain.remove(Strategy(m_player, m_cards).pickOptimalSeqSingles());

    auto findBeatCard = [this, hand, nextPlayer](const Cards& cards) -> Cards
    {
        QVector<Cards> beatCardsArray = Strategy(m_player, cards).findCardType(hand, true);
        if (beatCardsArray.isEmpty()) return Cards();
        return (m_player->getRole() != nextPlayer->getRole() && nextPlayer->getCards().cardCount() <= 2) ? beatCardsArray.back() : beatCardsArray.front();
    };

    Cards cards;
    if (!(cards = findBeatCard(remain)).isEmpty()) return cards;
    if (!(cards = findBeatCard(m_cards)).isEmpty()) return cards;

    return cards;
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
    case PlayHand::HandType::Hand_Single:
        return getCardsByCountFromPoint(beginPoint, 1); // BUG：按道理应该相同点数但是花色大的也加入，先这样子
    case PlayHand::HandType::Hand_Pair:
        return getCardsByCountFromPoint(beginPoint, 2);
    case PlayHand::HandType::Hand_Triple:
        return getCardsByCountFromPoint(beginPoint, 3);
    case PlayHand::HandType::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint, false);
    case PlayHand::HandType::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint, true);
    case PlayHand::HandType::Hand_Plane:
        return getPlane(beginPoint);
    case PlayHand::HandType::Hand_Plane_Two_Single:
        return getPlane2SingleOr2Pair(beginPoint, false);
    case PlayHand::HandType::Hand_Plane_Two_Pair:
        return getPlane2SingleOr2Pair(beginPoint, true);
    case PlayHand::HandType::Hand_Seq_Single:
        return getSeqSingleOrSepPair(beginPoint, extra, isBeat, false);
    case PlayHand::HandType::Hand_Seq_Pair:
        return getSeqSingleOrSepPair(beginPoint, extra, isBeat, true);
    case PlayHand::HandType::Hand_Bomb:
        return getBomb(beginPoint);
    default:
        return QVector<Cards>();
    }
}

void Strategy::pickSeqSingles(QVector<QVector<Cards>>& allSeqRecord, const Cards& cards, const QVector<Cards>& seqSingle) const
{
    // 1. 得到所有顺子的组合
    QVector<Cards> allSeq = Strategy(m_player, cards).findCardType(PlayHand(PlayHand::HandType::Hand_Seq_Single, Card::CardPoint::Card_Begin), false);
    if (allSeq.isEmpty())
    {
        // 结束递归，将满足条件的顺子传递给调用者
        allSeqRecord << seqSingle;
        return;
    }

    // 2. 对顺子进行筛选
    Cards saveCards = cards;
    // 遍历得到的所有的顺子
    for (int i = 0, size = (int)allSeq.size(); i < size; ++i)
    {
        // 将顺子取出
        Cards aScheme = allSeq.at(i);
        // 将顺子从用户手中删除
        Cards temp = saveCards;
        temp.remove(aScheme);

        QVector<Cards> seqArray = seqSingle;
        seqArray << aScheme;

        // 检测还有没有其他的顺子
        // seqArray 存储一轮for循环中多轮递归得到的所有的可用的顺子
        // allSeqRecord 存储多轮for循环中多轮递归得到的所有的可用的顺子
        pickSeqSingles(allSeqRecord, temp, seqArray);
    }
}

QVector<Cards> Strategy::pickOptimalSeqSingles() const
{
    QVector<QVector<Cards>> seqRecord;
    Cards save = m_cards;
    save.remove(findCardsByCount(4));
    save.remove(findCardsByCount(3));
    pickSeqSingles(seqRecord, save);
    if (seqRecord.isEmpty()) return QVector<Cards>();

    // 遍历容器
    QMap<int, int> seqMarks;
    for (int i = 0, size = (int)seqRecord.size(); i < size; ++i)
    {
        Cards backupCards = m_cards;
        QVector<Cards> seqArray = seqRecord[i];
        backupCards.remove(seqArray);

        // 判断剩下的单牌是数量，数量越少，顺子的组合就越合理
        QVector<Cards> singleArray = Strategy(m_player, backupCards).findCardsByCount(1);

        CardList cardList;
        for (int j = 0, size = (int)singleArray.size(); j < size; ++j) cardList << singleArray[j].toCardList();

        // 找点数相对较大一点顺子
        int mark = 0;
        for (int j = 0, size = (int)cardList.size(); j < size; ++j) mark += (int)cardList[j].getCardPoint() + 15;
        seqMarks.insert(i, mark);
    }

    // 遍历map
    int value = 0, comMark = INT_MAX;
    for (auto it = seqMarks.constBegin(); it != seqMarks.constEnd(); ++it)
    {
        if (it.value() < comMark)
        {
            comMark = it.value();
            value = it.key();
        }
    }

    return seqRecord[value];
}

QVector<Cards> Strategy::getCardsByCountFromPoint(Card::CardPoint point, int number) const
{
    QVector<Cards> findCardsArray;

    if (number < 1 || number > 4) return findCardsArray;

    for (; point < Card::CardPoint::Card_End; ++point)
    {
        // 只有刚刚好point的牌数等于number才加入，目的是尽量不拆分别的牌型（如炸弹）
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

    getBaseSeqSingleOrPair = [this, number, baseFollowed](Card::CardPoint point) -> Cards
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
        for (bool found; begin < end; ++begin)
        {
            found = true;
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

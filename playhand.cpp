#include "playhand.h"

PlayHand::PlayHand() {}

PlayHand::PlayHand(Cards& cards)
{
    // 1. 对扑克牌进行分类：1张，2张，3张，4张有多少种
    classify(cards);
    // 2. 对牌型进行分类
    judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint point, int extra) : m_type(type), m_point(point), m_extra(extra) {}

void PlayHand::classify(const Cards& cards)
{
    CardList list = cards.toCardList();
    int cardRecord[(int)Card::CardPoint::Card_BJ];
    memset(cardRecord, 0, sizeof(int) * (int)Card::CardPoint::Card_BJ);

    for (int i = 0, size = list.size(); i < size; ++i)
    {
        Card c = list.at(i);
        ++cardRecord[(int)c.getCardPoint()];
    }

    m_oneCard.clear();
    m_twoCard.clear();
    m_threeCard.clear();
    m_fourCard.clear();

    for (Card::CardPoint i = Card::CardPoint::Card_Begin; i < Card::CardPoint::Card_BJ - 1; ++i)
    {
        switch (cardRecord[(int)i])
        {
        case 1:
            m_oneCard.push_back(i);
            break;
        case 2:
            m_twoCard.push_back(i);
            break;
        case 3:
            m_threeCard.push_back(i);
            break;
        case 4:
            m_fourCard.push_back(i);
            break;
        }
    }
}

void PlayHand::judgeCardType()
{
    m_type = HandType::Hand_Unknown;
    m_point = Card::CardPoint::Card_Begin;
    m_extra = 0;

    if (isPass())
    {
        m_type = HandType::Hand_Pass;
    }
    else if (isSingle())
    {
        m_type = HandType::Hand_Single;
        m_point = m_oneCard[0];
    }
    else if (isPair())
    {
        m_type = HandType::Hand_Pair;
        m_point = m_twoCard[0];
    }
    else if (isTriple())
    {
        m_type = HandType::Hand_Triple;
        m_point = m_threeCard[0];
    }
    else if (isTripleSingle())
    {
        m_type = HandType::Hand_Triple_Single;
        m_point = m_threeCard[0];
    }
    else if (isTriplePair())
    {
        m_type = HandType::Hand_Triple_Pair;
        m_point = m_threeCard[0];
    }
    else if (isPlane())
    {
        m_type = HandType::Hand_Plane;
        // 记录点数最小的牌
        m_point = m_threeCard[0];
    }
    else if (isPlaneTwoSingle())
    {
        m_type = HandType::Hand_Plane_Two_Single;
        // 记录点数最小的牌
        m_point = m_threeCard[0];
    }
    else if (isPlaneTwoPair())
    {
        m_type = HandType::Hand_Plane_Two_Pair;
        // 记录点数最小的牌
        m_point = m_threeCard[0];
    }
    else if (isSeqPair())
    {
        m_type = HandType::Hand_Seq_Pair;
        // 记录点数最小的牌
        m_point = m_twoCard[0];
        m_extra = m_twoCard.size();
    }
    else if (isSeqSingle())
    {
        m_type = HandType::Hand_Seq_Single;
        // 记录点数最小的牌
        m_point = m_oneCard[0];
        m_extra = m_oneCard.size();
    }
    else if (isBomb())
    {
        m_type = HandType::Hand_Bomb;
        // 记录点数最小的牌
        m_point = m_fourCard[0];
    }
    else if (isBombSingle())
    {
        m_type = HandType::Hand_Bomb_Single;
        // 记录点数最小的牌
        m_point = m_fourCard[0];
    }
    else if (isBombPair())
    {
        m_type = HandType::Hand_Bomb_Pair;
        // 记录点数最小的牌
        m_point = m_fourCard[0];
    }
    else if (isBombTwoSingle())
    {
        m_type = HandType::Hand_Bomb_Two_Single;
        // 记录点数最小的牌
        m_point = m_fourCard[0];
    }
    else if (isBombJokers())
    {
        m_type = HandType::Hand_Bomb_Jokers;
    }
    else if (isBombJokersSingle())
    {
        m_type = HandType::Hand_Bomb_Jokers_Single;
    }
    else if (isBombJokersPair())
    {
        m_type = HandType::Hand_Bomb_Jokers_Pair;
    }
    else if (isBombJokersTwoSingle())
    {
        m_type = HandType::Hand_Bomb_Jokers_Two_Single;
    }
}

bool PlayHand::isPass() const
{
    if (m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isSingle() const
{
    if (m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPair() const
{
    if (m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriple() const
{
    if (m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTripleSingle() const
{
    if (m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriplePair() const
{
    if (m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPlane()
{
    if (m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end(), std::less<Card::CardPoint>());
        if ((int)m_threeCard[1] - (int)m_threeCard[0] == 1 && m_threeCard[1] < Card::CardPoint::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoSingle()
{
    if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end(), std::less<Card::CardPoint>());
        std::sort(m_threeCard.begin(), m_threeCard.end(), std::less<Card::CardPoint>());
        if ((int)m_threeCard[1] - (int)m_threeCard[0] == 1 && m_threeCard[1] < Card::CardPoint::Card_2 &&
            m_oneCard[0] != Card::CardPoint::Card_SJ && m_oneCard[1] != Card::CardPoint::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoPair()
{
    if (m_oneCard.isEmpty() && m_twoCard.size() == 2 && m_threeCard.size() == 2 && m_fourCard.isEmpty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end(), std::less<Card::CardPoint>());
        if ((int)m_threeCard[1] - (int)m_threeCard[0] == 1 && m_threeCard[1] < Card::CardPoint::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqPair()
{
    if (m_oneCard.isEmpty() && m_twoCard.size() >= 3 && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_twoCard.begin(), m_twoCard.end(), std::less<Card::CardPoint>());
        if ((int)m_twoCard.last() - (int)m_twoCard.first() == (m_twoCard.size() - 1) &&
            m_twoCard.first() >= Card::CardPoint::Card_3 && m_twoCard.last() < Card::CardPoint::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqSingle()
{
    if (m_oneCard.size() >= 5 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end(), std::less<Card::CardPoint>());
        if ((int)m_oneCard.last() - (int)m_oneCard.first() == (m_oneCard.size() - 1) &&
            m_oneCard.first() >= Card::CardPoint::Card_3 && m_oneCard.last() < Card::CardPoint::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBomb() const
{
    if (m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombSingle() const
{
    if (m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombPair() const
{
    if (m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombTwoSingle()
{
    if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        std::sort(m_oneCard.begin(), m_oneCard.end(), std::less<Card::CardPoint>());
        if (m_oneCard[0] != Card::CardPoint::Card_SJ && m_oneCard[1] != Card::CardPoint::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokers()
{
    if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end(), std::less<Card::CardPoint>());
        if (m_oneCard[0] == Card::CardPoint::Card_SJ && m_oneCard[1] == Card::CardPoint::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersSingle()
{
    if (m_oneCard.size() == 3 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end(), std::less<Card::CardPoint>());
        if (m_oneCard[1] == Card::CardPoint::Card_SJ && m_oneCard[2] == Card::CardPoint::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersPair()
{
    if (m_oneCard.size() == 2 && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end(), std::less<Card::CardPoint>());
        if (m_oneCard[0] == Card::CardPoint::Card_SJ && m_oneCard[1] == Card::CardPoint::Card_BJ)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersTwoSingle()
{
    if (m_oneCard.size() == 4 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end(), std::less<Card::CardPoint>());
        if (m_oneCard[2] == Card::CardPoint::Card_SJ && m_oneCard[3] == Card::CardPoint::Card_BJ)
        {
            return true;
        }
    }
    return false;
}
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

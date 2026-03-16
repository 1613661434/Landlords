#include "playhand.h"

PlayHand::PlayHand() {}

PlayHand::PlayHand(Cards& cards)
{
    // 1. 对扑克牌进行分类：1张，2张，3张，4张有多少种
    classify(cards);
    // 2. 对牌型进行分类
    judgeCardType();
}

PlayHand::PlayHand(PlayHand::HandType type, Card::CardPoint point, int extra) : m_type(type), m_point(point), m_extra(extra) {}

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

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

void PlayHand::classify(Cards& cards)
{
}

void PlayHand::judgeCardType()
{
}

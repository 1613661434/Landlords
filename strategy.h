#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "playhand.h"

class Strategy
{
private:
    Player* m_player;
    Cards m_cards;

public:
    Strategy(Player* player, const Cards& cards);

    // 1. 制定出牌策略
    Cards makeStrategy();
    // 2. 第一个出牌 firstPlay
    Cards firstPlay();
    // 3. 得到比指定牌型大的牌
    Cards getGreaterCards(PlayHand type);
    // 4. 能大过指定的牌时，判断是出牌还是放行, 返回true->出牌, 返回false->放行
    bool whetherToBeat(Cards& cs);

    // 5. 找出指定数量(count)的相同点数的牌(point), 找出count张点数为point的牌
    Cards findSamePointCards(Card::CardPoint point, int count) const;
    // 6. 找出所有点数数量为count的牌 ==> 得到一个多张扑克牌数组
    QVector<Cards> findCardsByCount(int count) const;
    // 7. 根据点数范围找牌
    Cards getRangeCards(Card::CardPoint begin, Card::CardPoint end) const;
    // 8. 按牌型找牌，并且指定要找的牌是否要大过指定的牌型
    QVector<Cards> findCardType(PlayHand hand, bool beat) const;

private:
    // 从指定起始点数开始，向后查找所有数量恰好为number的同点数牌组
    QVector<Cards> findCardsByCountFromPoint(Card::CardPoint point, int number) const;
};

#endif // STRATEGY_H

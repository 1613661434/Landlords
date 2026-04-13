#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "playhand.h"

class Strategy
{
private:
    Player* m_player = nullptr;
    Cards m_cards;

public:
    Strategy(Player* player, const Cards& cards);

    // 1. 制定出牌策略
    Cards makeStrategy() const;

private:
    // 1.1. 第一个出牌
    Cards firstPlay() const;
    // 1.2. 得到比指定牌型大的牌
    Cards getGreaterCards(PlayHand hand) const;
    // 1.3. 能大过指定的牌时，判断是出牌还是放行, 返回true->出牌, 返回false->放行
    bool whetherToBeat(const Cards& cards) const;

public:
    // 2. 找出指定数量(count)的相同点数的牌(point), 找出count张点数为point的牌
    Cards findSamePointCards(Card::CardPoint point, int count) const;
    // 3. 找出所有点数数量为count的牌 ==> 得到一个多张扑克牌数组
    QVector<Cards> findCardsByCount(int count) const;
    // 4. 根据点数范围找牌
    Cards getRangeCards(Card::CardPoint begin, Card::CardPoint end) const;
    // 5. 按牌型找牌，并且指定要找的牌是否要大过指定的牌型
    QVector<Cards> findCardType(PlayHand hand, bool isBeat) const;
    // 6. 从指定的Cards对象中挑选出满足条件的顺子
    void pickSeqSingles(QVector<QVector<Cards>>& allSeqRecord, const Cards& cards, const QVector<Cards>& seqSingle = QVector<Cards>()) const;
    // 7. 最优的顺子的集合的筛选函数
    QVector<Cards> pickOptimalSeqSingles() const;

private:
    // 从指定起始点数开始，向后查找所有数量恰好为number的同点数牌组
    QVector<Cards> getCardsByCountFromPoint(Card::CardPoint point, int number) const;
    // 找三带一或者三带二
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin, bool isPair) const;
    // 找飞机
    QVector<Cards> getPlane(Card::CardPoint begin) const;
    // 找飞机带两单或两对
    QVector<Cards> getPlane2SingleOr2Pair(Card::CardPoint begin, bool isPair) const;
    // 找连对或顺子
    QVector<Cards> getSeqSingleOrSepPair(Card::CardPoint begin, int extra, bool isBeat, bool isPair) const;
    // 找炸弹
    QVector<Cards> getBomb(Card::CardPoint begin) const;
};

#endif // STRATEGY_H

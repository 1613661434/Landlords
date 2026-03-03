#include "gamecontrol.h"
#include <QRandomGenerator>

GameControl::GameControl(QObject* parent)
    : QObject{parent}
{
}

void GameControl::playerInit()
{
    // 对象实例化
    m_robotLeft = new Robot("机器人A", this);
    m_robotRight = new Robot("机器人B", this);
    m_user = new UserPlayer("我自己", this);

    // 头像的显示
    m_robotLeft->setDirection(Player::Direction::Left);
    m_robotRight->setDirection(Player::Direction::Right);
    m_user->setDirection(Player::Direction::Right);

    // 性别
    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);

    // 出牌顺序
    // user
    m_user->setPrevPlayer(m_robotLeft);
    m_user->setNextPlayer(m_robotRight);

    // left robot
    m_robotLeft->setPrevPlayer(m_robotRight);
    m_robotLeft->setNextPlayer(m_user);

    // right robot
    m_robotRight->setPrevPlayer(m_user);
    m_robotRight->setNextPlayer(m_robotLeft);

    // 指定当前玩家
    m_currPlayer = m_user;
}

void GameControl::initAllCards()
{
    m_allCards.clear();
    for (Card::CardPoint p = Card::CardPoint::Card_Begin + 1; p < Card::Card::CardPoint::Card_SJ; ++p)
    {
        for (Card::CardSuit s = Card::CardSuit::Suit_Begin + 1; s < Card::CardSuit::Suit_End; ++s)
        {
            Card c(p, s);
            m_allCards.add(c);
        }
    }
    m_allCards.add(Card(Card::CardPoint::Card_SJ, Card::CardSuit::Suit_Begin));
    m_allCards.add(Card(Card::CardPoint::Card_BJ, Card::CardSuit::Suit_Begin));
}

void GameControl::resetCardData()
{
    // 洗牌
    initAllCards();
    // 清空所有玩家的牌
    m_robotLeft->clearCards();
    m_robotRight->clearCards();
    m_user->clearCards();
    // 初始化出牌玩家和牌
    m_pendPlayer = nullptr;
    m_pendCards.clear();
}

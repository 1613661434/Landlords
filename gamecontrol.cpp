#include "gamecontrol.h"
#include <QRandomGenerator>
#include <QTimer>

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

    // 处理玩家发射出的信号
    connect(m_user, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotLeft, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotRight, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
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

void GameControl::becomeLord(Player* player)
{
    player->setRole(Player::Role::Load);
    player->getPrevPlayer()->setRole(Player::Role::Farmer);
    player->getNextPlayer()->setRole(Player::Role::Farmer);

    m_currPlayer = player;
    player->storeDispatchCard(m_allCards);

    QTimer::singleShot(1000, this, [this]()
                       {
        emit gameStatusChanged(GameControl::GameStatus::PlayingHand);
        emit playerStatusChanged(m_currPlayer,GameControl::PlayerStatus::ThinkingForPlayHand);
        m_currPlayer->preparePlayHand(); });
}

void GameControl::clearPlayerScore()
{
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
    m_user->setScore(0);
}

void GameControl::onGrabBet(Player* player, int point)
{
    // 1. 通知主界面玩家叫地主了（更新信息提示）
    if (point == 0 || m_betRecord.point >= point)
    {
        // 不抢或无效的
        emit notifyGrabLordBet(player, 0, false);
    }
    else if (point > 0 && m_betRecord.point == 0)
    {
        // 第一个叫地主的玩家
        emit notifyGrabLordBet(player, point, true);
    }
    else
    {
        // 第二三个抢地主的玩家
        emit notifyGrabLordBet(player, point, false);
    }
    // 2. 判断玩家下注是不是3分，如果是抢地主结束
    if (point == 3)
    {
        // 玩家成为地主
        becomeLord(player);
        // 清空数据
        m_betRecord.reset();
        return;
    }
    // 3. 下注不够3分，对玩家的分数进行比较，分数高的是地主
    if (m_betRecord.point < point)
    {
        m_betRecord.point = point;
        m_betRecord.player = player;
    }
    ++m_betRecord.times;
    // 如果每个玩家都抢过一次地主，抢地主结束
    if (m_betRecord.times == 3)
    {
        if (m_betRecord.point == 0)
        {
            emit gameStatusChanged(GameControl::GameStatus::DispatchCard);
        }
        else
        {
            becomeLord(m_betRecord.player);
        }
        m_betRecord.reset();
        return;
    }
    // 4. 切换玩家，通知下一个玩家继续抢地主
    m_currPlayer = player->getNextPlayer();
    // 发送信号给主界面，告知当前状态为抢地主
    emit playerStatusChanged(m_currPlayer, GameControl::PlayerStatus::ThinkingForCallLord);
    m_currPlayer->prepareCallLord();
}

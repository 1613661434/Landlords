#include "gamecontrol.h"
#include "playhand.h"
#include <QRandomGenerator>
#include <QTimer>

GameControl::GameControl(QObject* parent)
    : QObject{parent}
{
}

void GameControl::playerInit()
{
    // 对象实例化
    m_user = new UserPlayer("Me", this);
    m_robotLeft = new Robot("Robot_Left", this);
    m_robotRight = new Robot("Robot_Right", this);

    // 头像的显示
    m_user->setDirection(Player::Direction::Right);
    m_robotLeft->setDirection(Player::Direction::Left);
    m_robotRight->setDirection(Player::Direction::Right);

    // 性别
    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);

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
    connect(m_robotLeft, &Robot::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotRight, &Robot::notifyGrabLordBet, this, &GameControl::onGrabBet);

    // 传递出牌玩家对象和玩家打出的牌
    connect(this, &GameControl::pendingInfo, m_user, &UserPlayer::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_robotLeft, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_robotRight, &Robot::storePendingInfo);

    // 处理玩家出牌
    connect(m_user, &UserPlayer::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_robotLeft, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_robotRight, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
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

void GameControl::becomeLord(Player* player, int point)
{
    player->setRole(Player::Role::Lord);
    player->getPrevPlayer()->setRole(Player::Role::Farmer);
    player->getNextPlayer()->setRole(Player::Role::Farmer);

    m_currPlayer = player;
    m_lordBetPoint = point;
    player->storeDispatchCard(m_allCards);

    QTimer::singleShot(1000, this, [this]()
                       {
        emit gameStatusChanged(GameStatus::PlayingHand);
        emit playerStatusChanged(m_currPlayer,PlayerStatus::ThinkingForPlayHand);
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
    if (point == 0 || m_betRecord.point >= point) // 不抢或无效的
        emit notifyGrabLordBet(player, 0, false);
    else if (point > 0 && m_betRecord.point == 0) // 第一个叫地主的玩家
        emit notifyGrabLordBet(player, point, true);
    else // 第二三个抢地主的玩家
        emit notifyGrabLordBet(player, point, false);

    // 2. 判断玩家下注是不是3分，如果是抢地主结束
    if (point == 3)
    {
        // 玩家成为地主
        becomeLord(player, point);
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
            emit gameStatusChanged(GameControl::GameStatus::DispatchCard);
        else
            becomeLord(m_betRecord.player, m_betRecord.point);

        m_betRecord.reset();
        return;
    }

    // 4. 切换玩家，通知下一个玩家继续抢地主
    m_currPlayer = player->getNextPlayer();
    // 发送信号给主界面，告知当前状态为抢地主
    emit playerStatusChanged(m_currPlayer, GameControl::PlayerStatus::ThinkingForCallLord);
    m_currPlayer->prepareCallLord();
}

void GameControl::onPlayHand(Player* player, Cards cards)
{
    // 1. 将玩家出牌的信号转发给主界面
    emit notifyPlayHand(player, cards);

    // 2. 如果不是空牌, 给其他玩家发送信号, 保存出牌玩家对象和打出的牌
    if (!cards.isEmpty())
    {
        m_pendCards = cards;
        m_pendPlayer = player;
        emit pendingInfo(player, cards);
    }

    // 如果有炸弹, 底分翻倍
    PlayHand::HandType type = PlayHand(cards).getHandType();
    if (type == PlayHand::HandType::Hand_Bomb || type == PlayHand::HandType::Hand_Bomb_Jokers) m_lordBetPoint *= 2;

    // 3. 如果玩家的牌出完了, 计算本局游戏的总分
    if (player->getCards().isEmpty())
    {
        Player* prev = player->getPrevPlayer();
        Player* next = player->getNextPlayer();
        if (player->getRole() == Player::Role::Lord)
        {
            prev->setScore(prev->getScore() - m_lordBetPoint);
            next->setScore(next->getScore() - m_lordBetPoint);
            player->setScore(player->getScore() + 2 * m_lordBetPoint);
            prev->setIsWin(false);
        }
        else
        {
            if (prev->getRole() == Player::Role::Lord)
            {
                Player* temp = prev;
                prev = next;
                next = temp;
            }
            prev->setScore(prev->getScore() + m_lordBetPoint);
            next->setScore(next->getScore() - 2 * m_lordBetPoint);
            player->setScore(player->getScore() + m_lordBetPoint);
            prev->setIsWin(true);
        }
        next->setIsWin(false);
        player->setIsWin(true);
        emit playerStatusChanged(player, PlayerStatus::Winning);
        return;
    }

    // 4. 牌没有出完, 下一个玩家继续出牌
    m_currPlayer = player->getNextPlayer();
    m_currPlayer->preparePlayHand();
    emit playerStatusChanged(m_currPlayer, PlayerStatus::ThinkingForPlayHand);
}

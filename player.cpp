#include "player.h"

Player::Player(QObject* parent)
    : QObject{parent},
      m_score(0),
      m_isWin(false),
      m_prev(nullptr),
      m_next(nullptr),
      m_pendPlayer(nullptr)
{
}

Player::Player(QString name, QObject* parent) : Player(parent)
{
    m_name = name;
}

void Player::setRole(Role role)
{
    QMutexLocker locker(&m_roleMutex);
    m_role = role;
}

Player::Role Player::getRole() const
{
    QMutexLocker locker(&m_roleMutex);
    return m_role;
}

void Player::storeDispatchCard(Card& card)
{
    QMutexLocker locker(&m_cardMutex);
    m_cards.add(card);
}

void Player::storeDispatchCard(Cards& cards)
{
    QMutexLocker locker(&m_cardMutex);
    m_cards.add(cards);
}

Cards Player::getCards() const
{
    QMutexLocker locker(&m_cardMutex);
    return m_cards;
}

void Player::clearCards()
{
    QMutexLocker locker(&m_cardMutex);
    m_cards.clear();
}

void Player::playHand(Cards& cards)
{
    QMutexLocker locker(&m_cardMutex);
    m_cards.remove(cards);
    emit notifyPlayHand(this, cards);
}
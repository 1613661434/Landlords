#include "player.h"

Player::Player(QObject* parent)
    : QObject{parent}
{
}

Player::Player(QString name, QObject* parent) : m_name(name)
{
}

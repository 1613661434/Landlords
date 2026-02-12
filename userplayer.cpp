#include "userplayer.h"

UserPlayer::UserPlayer(QObject* parent)
    : Player{parent}
{
    m_type = Type::User;
}

void UserPlayer::prepareCallLord()
{
}

void UserPlayer::preparePlayHand()
{
}

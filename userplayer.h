#ifndef USERPLAYER_H
#define USERPLAYER_H

#include <QObject>
#include "player.h"

class UserPlayer : public Player
{
    Q_OBJECT

public:
    using Player::Player;
    explicit UserPlayer(QObject* parent = nullptr);

    void prepareCallLord() override;
    void preparePlayHand() override;

    // 考虑叫地主
    void thinkCallLord() override;
    // 考虑出牌
    void thinkPlayHand() override;

signals:
    void startCountDown();
};

#endif // USERPLAYER_H

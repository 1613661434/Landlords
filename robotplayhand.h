#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include <QThread>
#include "robot.h"

class RobotPlayHand : public QThread
{
    Q_OBJECT
public:
    explicit RobotPlayHand(Robot* robot, QObject* parent = nullptr);

private:
    Robot* m_robot;

protected:
    void run() override;

signals:
};

#endif // ROBOTPLAYHAND_H

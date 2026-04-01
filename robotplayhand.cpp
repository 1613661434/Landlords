#include "robotplayhand.h"

RobotPlayHand::RobotPlayHand(Robot* robot, QObject* parent)
    : m_robot(robot), QThread{parent}
{
}

void RobotPlayHand::run()
{
    msleep(1500);
    m_robot->thinkPlayHand();
}

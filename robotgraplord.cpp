#include "robotgraplord.h"

RobotGrapLord::RobotGrapLord(Robot* robot, QObject* parent)
    : m_robot(robot), QThread{parent}
{
}

void RobotGrapLord::run()
{
    msleep(1500);
    m_robot->thinkCallLord();
}

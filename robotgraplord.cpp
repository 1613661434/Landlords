#include "robotgraplord.h"

RobotGrapLord::RobotGrapLord(Robot* robot, QThread* parent)
    : m_robot(robot), QThread{parent}
{
}

void RobotGrapLord::run()
{
    msleep(2000);
    m_robot->thinkCallLord();
}

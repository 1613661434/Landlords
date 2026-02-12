#include "robot.h"
#include "robot.h"

Robot::Robot(QObject* parent)
    : Player{parent}
{
    m_type = Type::Robot;
}

void Robot::prepareCallLord()
{
}

void Robot::preparePlayHand()
{
}

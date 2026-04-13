#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H

#include <QThread>
#include "robot.h"

class RobotGrapLord : public QThread
{
    Q_OBJECT
private:
    Robot* m_robot = nullptr;

public:
    explicit RobotGrapLord(Robot* robot, QObject* parent = nullptr);

protected:
    void run() override;

signals:
};

#endif // ROBOTGRAPLORD_H

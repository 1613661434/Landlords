#include "gamepanel.h"
#include "ui_gamepanel.h"
#include <QRandomGenerator>

GamePanel::GamePanel(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::GamePanel)
{
    ui->setupUi(this);

    ui->btnGroup->initButtons();
    ui->btnGroup->selectPanel(ButtonGroup::Panel::Start);

    // 1. 背景图
    int num = QRandomGenerator::global()->bounded(1, 11); // 1-10
    QString path = QString(":/images/background-%1.png").arg(num);
    m_bkImage.load(path);
    // 2. 窗口的标题的大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000, 650);
    // 3. 实例化游戏控制类对象
    gameControlInit();
    // 4. 更新玩家得分
    updatePlayerScore();
    // 5. 切割游戏图片
    // 6. 初始化游戏中的按钮组
    // 7. 初始化玩家在窗口中的上下文环境
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::gameControlInit()
{
    m_gameCtl = new GameControl(this);
    m_gameCtl->playerInit();
    // 得到三个玩家的实例对象（顺序：左侧机器人，右侧机器人，当前玩家）
    m_playerList << m_gameCtl->getLeftRobot() << m_gameCtl->getRightRobot() << m_gameCtl->getUserPlayer();
}

void GamePanel::updatePlayerScore()
{
    ui->scorePanel->setScores(m_playerList[0]->getScore(), m_playerList[1]->getScore(), m_playerList[2]->getScore());
}

void GamePanel::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_bkImage);
}

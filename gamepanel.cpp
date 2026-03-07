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
    // 4. 玩家得分
    // 5. 切割游戏图片
    // 6. 初始化游戏中的按钮组
    // 7. 初始化玩家在窗口中的上下文环境
}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_bkImage);
}

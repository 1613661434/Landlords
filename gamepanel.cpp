#include "gamepanel.h"
#include "ui_gamepanel.h"
#include <QRandomGenerator>
#include "ol_enum_char_ops.h"

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
    initCardMap();
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

void GamePanel::initCardMap()
{
    // 1. 加载大图
    QPixmap pixmap(":/images/card.png");
    // 2. 计算每张图片大小
    m_cardSize.setWidth(pixmap.width() / 13);
    m_cardSize.setHeight(pixmap.height() / 5);
    // 3. 加载卡牌图片
    // 背景图
    m_cardBackImg = pixmap.copy(2 * m_cardSize.width(), 4 * m_cardSize.height(), m_cardSize.width(), m_cardSize.height());
    // 正常花色
    for (Card::CardSuit suit = Card::CardSuit::Suit_Begin + 1; suit < Card::CardSuit::Suit_End; ++suit)
    {
        for (Card::CardPoint point = Card::CardPoint::Card_Begin + 1; point < Card::CardPoint::Card_SJ; ++point)
        {
            Card card(point, suit);
            cropImage(pixmap, (int)(point - 1) * m_cardSize.width(), (int)(suit - 1) * m_cardSize.height(), card);
        }
    }
    // 大小王
    Card card(Card::CardPoint::Card_SJ, Card::CardSuit::Suit_Begin);
    cropImage(pixmap, 0, 4 * m_cardSize.height(), card);
    card.setCardPoint(Card::CardPoint::Card_BJ);
    cropImage(pixmap, m_cardSize.width(), 4 * m_cardSize.height(), card);
}

void GamePanel::cropImage(const QPixmap& pixmap, int x, int y, const Card& card)
{
    QPixmap sub = pixmap.copy(x, y, m_cardSize.width(), m_cardSize.height());
    CardPanel* panel = new CardPanel(this);
    panel->setImage(sub, m_cardBackImg);
    panel->setCard(card);
    panel->hide();
    m_cardMap.insert(card, panel);
}

void GamePanel::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_bkImage);
}

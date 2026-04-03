#include "gamepanel.h"
#include "ui_gamepanel.h"
#include "ol_enum_char_ops.h"
#include "playhand.h"
#include <QRandomGenerator>

GamePanel::GamePanel(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::GamePanel)
{
    ui->setupUi(this);

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
    initButtonsGroup();
    // 7. 初始化玩家在窗口中的上下文环境
    initPlayerContext();
    // 8. 扑克牌场景初始化
    initGameScene();

    // 定时器实例化
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GamePanel::onDispatchCard);

    // 动画效果窗口类实例化
    m_animation = new AnimationWindow(this);
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

    connect(m_gameCtl, &GameControl::playerStatusChanged, this, &GamePanel::onPlayerStatusChanged);
    connect(m_gameCtl, &GameControl::notifyGrabLordBet, this, &GamePanel::onGrabLordBet);
    connect(m_gameCtl, &GameControl::gameStatusChanged, this, &GamePanel::gameStatusPrecess);
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

void GamePanel::initButtonsGroup()
{
    ui->btnGroup->initButtons();
    ui->btnGroup->selectPanel(ButtonGroup::Panel::Start);

    connect(ui->btnGroup, &ButtonGroup::startGame, this, [this]()
            {
                // 界面的初始化
                ui->btnGroup->selectPanel(ButtonGroup::Panel::Empty);
                m_gameCtl->clearPlayerScore();
                updatePlayerScore();
                // 修改游戏状态 -> 发牌
                gameStatusPrecess(GameControl::GameStatus::DispatchCard); });
    connect(ui->btnGroup, &ButtonGroup::playHand, this, [this]() {});
    connect(ui->btnGroup, &ButtonGroup::pass, this, [this]() {});
    connect(ui->btnGroup, &ButtonGroup::betPoint, this, [this](int point)
            { m_gameCtl->getUserPlayer()->grabLordBet(point);
             ui->btnGroup->selectPanel(ButtonGroup::Panel::Empty); });
}

void GamePanel::initPlayerContext()
{
    // 1. 放置玩家扑克牌的区域
    const QRect cardsRect[] =
        {
            // x, y, width, height
            QRect(90, 130, 100, height() - 200),                   // 左侧机器人
            QRect(rect().right() - 190, 130, 100, height() - 200), // 右侧机器人
            QRect(250, rect().bottom() - 120, width() - 500, 100)  // 当前玩家
        };
    // 2. 玩家出牌的区域
    const QRect playHandRect[] =
        {
            QRect(260, 150, 100, 100),                            // 左侧机器人
            QRect(rect().right() - 360, 150, 100, 100),           // 右侧机器人
            QRect(150, rect().bottom() - 290, width() - 300, 105) // 当前玩家
        };
    // 3. 玩家头像显示的位置
    const QPoint roleImgPos[] =
        {
            QPoint(cardsRect[0].left() - 80, cardsRect[0].height() / 2 + 20),  // 左侧机器人
            QPoint(cardsRect[1].right() + 10, cardsRect[1].height() / 2 + 20), // 右侧机器人
            QPoint(cardsRect[2].right() - 10, cardsRect[2].top() - 10)         // 当前玩家
        };

    // 循环
    const int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for (int i = 0, size = (int)m_playerList.size(); i < size; ++i)
    {
        PlayerContext context;
        context.align = i == index ? CardAlign::Horizontal : CardAlign::Vertical;
        context.isFrontSide = i == index ? true : false;
        context.cardRect = cardsRect[i];
        context.playHandRect = playHandRect[i];
        // 提示信息
        context.info = new QLabel(this);
        context.info->resize(160, 98);
        context.info->hide();
        // 显示到出牌区域的中心位置
        QRect rect = playHandRect[i];
        QPoint pt(rect.left() + (rect.width() - context.info->width()) / 2,
                  rect.top() + (rect.height() - context.info->height()) / 2);
        context.info->move(pt);
        // 玩家的头像
        context.roleImg = new QLabel(this);
        context.roleImg->resize(84, 120);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);
        m_contextMap.insert(m_playerList.at(i), context);
    }
}

void GamePanel::initGameScene()
{
    // 1. 发牌区的扑克牌
    m_baseCard = new CardPanel(this);
    m_baseCard->setImage(m_cardBackImg, m_cardBackImg);
    // 2. 发牌过程中移动的扑克牌
    m_moveCard = new CardPanel(this);
    m_moveCard->setImage(m_cardBackImg, m_cardBackImg);
    // 3. 最后的三张底牌（用于窗口的显示）
    m_last3Card.reserve(3);
    for (int i = 0; i < 3; ++i)
    {
        CardPanel* panel = new CardPanel(this);
        panel->setImage(m_cardBackImg, m_cardBackImg);
        panel->hide();
        m_last3Card.push_back(panel);
    }
    // 扑克牌的位置
    m_baseCardPos = QPoint((width() - m_cardSize.width()) / 2, height() / 2 - 100);
    m_baseCard->move(m_baseCardPos);
    m_moveCard->move(m_baseCardPos);

    int base = (width() - 3 * m_cardSize.width() - 2 * 10) / 2;
    for (int i = 0; i < 3; ++i) m_last3Card[i]->move(base + (m_cardSize.width() + 10) * i, 20);
}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status)
{
    // 记录游戏状态
    m_gameStatus = status;
    // 处理游戏状态
    switch (status)
    {
    case GameControl::GameStatus::DispatchCard:
        startDispatchCard();
        break;
    case GameControl::GameStatus::CallingLord:
    {
        // 取出底牌数据
        CardList last3Card = m_gameCtl->getSurplusCards().toCardList();
        // 给底牌窗口设置图片
        for (int i = 0, size = (int)last3Card.size(); i < size; ++i)
        {
            QPixmap front = m_cardMap[last3Card.at(i)]->getImage();
            m_last3Card[i]->setImage(front, m_cardBackImg);
            m_last3Card[i]->hide();
        }
        // 开始叫地主
        m_gameCtl->startLordCard();
        break;
    }
    case GameControl::GameStatus::PlayingHand:
        break;
    }
}

void GamePanel::startDispatchCard()
{
    // 重置每张卡牌的属性
    for (auto it = m_cardMap.begin(); it != m_cardMap.end(); ++it)
    {
        it.value()->setSelected(false);
        it.value()->setFrontSide(true);
        it.value()->hide();
    }
    // 隐藏三张底牌
    for (int i = 0, size = (int)m_last3Card.size(); i < size; ++i) m_last3Card.at(i)->hide();
    // 重置玩家的窗口上下文信息
    const int index = m_playerList.indexOf(m_gameCtl->getUserPlayer());
    for (int i = 0, size = (int)m_playerList.size(); i < size; ++i)
    {
        m_contextMap[m_playerList.at(i)].lastCards.clear();
        m_contextMap[m_playerList.at(i)].info->hide();
        m_contextMap[m_playerList.at(i)].roleImg->hide();
        m_contextMap[m_playerList.at(i)].isFrontSide = (i == index ? true : false);
    }
    // 重置所有玩家的卡牌数据
    m_gameCtl->resetCardData();
    // 显示底牌
    m_baseCard->show();
    // 隐藏按钮面板
    ui->btnGroup->selectPanel(ButtonGroup::Panel::Empty);
    // 启动定时器
    m_timer->start(10);
    // 播放背景音乐
}

void GamePanel::cardMoveStep(Player* player, int curPos)
{
    // 获取当前玩家的索引
    int index = m_playerList.indexOf(player);

    // 获取当前玩家的扑克牌展示区域
    QRect cardRect = m_contextMap[player].cardRect;

    // 计算当前玩家对应的单元步长
    int currentUnit;
    // 计算当前玩家对应的实时坐标
    QPoint currentPos;
    switch (index)
    {
    case 0:
        currentUnit = (m_baseCardPos.x() - cardRect.right()) / 100;
        currentPos = QPoint(m_baseCardPos.x() - curPos * currentUnit, m_baseCardPos.y());
        break;
    case 1:
        currentUnit = (cardRect.left() - m_baseCardPos.x()) / 100;
        currentPos = QPoint(m_baseCardPos.x() + curPos * currentUnit, m_baseCardPos.y());
        break;
    case 2:
        currentUnit = (cardRect.top() - m_baseCardPos.y()) / 100;
        currentPos = QPoint(m_baseCardPos.x(), m_baseCardPos.y() + curPos * currentUnit);
        break;
    }

    // 移动扑克牌窗口
    m_moveCard->move(currentPos);

    // 临界状态处理
    if (curPos == 0)
        m_moveCard->show();
    else if (curPos >= 100)
        m_moveCard->hide();
}

void GamePanel::disposCard(Player* player, const Cards& cards)
{
    CardList list = cards.toCardList();
    for (int i = 0, size = (int)list.size(); i < size; ++i)
    {
        CardPanel* panel = m_cardMap[list.at(i)];
        panel->setOwner(player);
    }
    // 更新扑克牌在窗口中的显示
    updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player* player)
{
    Cards cards = player->getCards();
    CardList list = cards.toCardList();
    // 取出展示扑克牌的区域
    int cardSpace = 20;
    QRect cardsRect = m_contextMap[player].cardRect;
    for (int i = 0, size = (int)list.size(); i < size; ++i)
    {
        CardPanel* panel = m_cardMap[list.at(i)];
        panel->show();
        panel->raise();
        panel->setFrontSide(m_contextMap[player].isFrontSide);

        // 水平 or 垂直显示
        if (m_contextMap[player].align == GamePanel::CardAlign::Horizontal)
        {
            int leftX = cardsRect.left() + (cardsRect.width() - (size - 1) * cardSpace - panel->width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - m_cardSize.height()) / 2;
            if (panel->isSelected()) topY -= 10;
            panel->move(leftX + cardSpace * i, topY);
        }
        else
        {
            int leftX = cardsRect.left() + (cardsRect.width() - m_cardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (size - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX, topY + i * cardSpace);
        }
    }
}

void GamePanel::onDispatchCard()
{
    // 记录扑克牌的位置
    static int curMovePos = 0;
    // 当前玩家
    Player* curPlayer = m_gameCtl->getCurrentPlayer();
    if (curMovePos >= 100)
    {
        // 给玩家发一张牌
        Card card = m_gameCtl->takeOneCard();
        curPlayer->storeDispatchCard(card);
        // 发牌动画
        disposCard(curPlayer, Cards(card));
        cardMoveStep(curPlayer, curMovePos);
        // 判断牌是否发完了
        if (m_gameCtl->getSurplusCards().cardCount() == 3)
        {
            // 终止定时器
            m_timer->stop();
            // 重置扑克牌位置
            curMovePos = 0;
            // 切换为用户玩家
            m_gameCtl->setCurrentPlayer(m_gameCtl->getUserPlayer());
            // 切换游戏状态 -> 叫地主
            gameStatusPrecess(GameControl::GameStatus::CallingLord);
            return;
        }
        // 重置扑克牌位置
        curMovePos = 0;
        // 切换玩家
        m_gameCtl->setCurrentPlayer(curPlayer->getNextPlayer());
        return;
    }
    // 移动扑克牌
    cardMoveStep(curPlayer, curMovePos);
    curMovePos += 15;
}

void GamePanel::onPlayerStatusChanged(Player* player, GameControl::PlayerStatus status)
{
    switch (status)
    {
    case GameControl::PlayerStatus::ThinkingForCallLord:
        if (player == m_gameCtl->getUserPlayer()) ui->btnGroup->selectPanel(ButtonGroup::Panel::CallLord, m_gameCtl->getPlayerMaxBet());
        break;
    case GameControl::PlayerStatus::ThinkingForPlayHand:
        break;
    case GameControl::PlayerStatus::Winning:
        break;
    }
}

void GamePanel::onGrabLordBet(Player* player, int point, bool isFirst)
{
    // 显示抢地主的信息提示
    PlayerContext context = m_contextMap[player];
    if (point == 0)
    {
        context.info->setPixmap(QPixmap(":/images/buqiang.png"));
    }
    else
    {
        if (isFirst)
            context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
        else
            context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
    }
    context.info->show();
    // 显示抢地主的分数
    showAnimation(AnimationType::Score, point);
    // 播放分数的背景音乐
}

void GamePanel::onDisposePlayHand(Player* player, Cards& cards)
{
    // 1. 隐藏上一轮打出的牌
    hidePlayerDropCards(player);
    // 存储玩家打出的牌
    auto it = m_contextMap.find(player);
    if (it != m_contextMap.end()) it->lastCards = cards;

    // 2. 根据牌型播放游戏特效
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getHandType();
    switch (type)
    {
    case PlayHand::HandType::Hand_Plane:
    case PlayHand::HandType::Hand_Plane_Two_Single:
    case PlayHand::HandType::Hand_Plane_Two_Pair:
        showAnimation(AnimationType::Plane);
        break;
    case PlayHand::HandType::Hand_Seq_Single:
        showAnimation(AnimationType::Seq_Single);
        break;
    case PlayHand::HandType::Hand_Seq_Pair:
        showAnimation(AnimationType::Seq_Pair);
        break;
    case PlayHand::HandType::Hand_Bomb:
        showAnimation(AnimationType::Bomb);
        break;
    case PlayHand::HandType::Hand_Bomb_Jokers:
        showAnimation(AnimationType::JokerBomb);
        break;
    default:
        break;
    }

    // 如果玩家打出的是空牌(不出牌), 显示提示信息
    if (cards.isEmpty())
    {
        it->info->setPixmap(QPixmap(":/images/pass.png"));
        it->info->show();
    }

    // 3. 更新玩家剩余的牌
    updatePlayerCards(player);

    // 4. 播放提示音乐
}

void GamePanel::showAnimation(AnimationType type, int point)
{
    switch (type)
    {
    case AnimationType::Score:
        m_animation->setFixedSize(160, 98);
        m_animation->move((width() - m_animation->width()) / 2, (height() - m_animation->height()) / 2 - 140);
        m_animation->showBetScore(point);
        return;
    case AnimationType::Seq_Single:
    case AnimationType::Seq_Pair:
        break;
    case AnimationType::Plane:
        break;
    case AnimationType::Bomb:
        break;
    case AnimationType::JokerBomb:
        break;
    }
    m_animation->show();
}

void GamePanel::hidePlayerDropCards(Player* player)
{
    auto it = m_contextMap.find(player);
    if (it == m_contextMap.end()) return;

    if (it->lastCards.isEmpty())
    {
        it->info->hide();
    }
    else
    {
        // Cards --> Card
        CardList list = it->lastCards.toCardList();
        for (const auto& card : list) m_cardMap[card]->hide();
        it->lastCards.clear();
    }
}

void GamePanel::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_bkImage);
}

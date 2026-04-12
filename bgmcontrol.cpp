#include "bgmcontrol.h"
#include "playhand.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QTimer>

BGMControl::BGMControl(QObject* parent) : QObject(parent)
{
    // 初始化5组 播放器 + 音频输出
    for (int i = 0; i < 5; ++i)
    {
        QAudioOutput* audioOut = new QAudioOutput(this);
        QMediaPlayer* player = new QMediaPlayer(this);
        player->setAudioOutput(audioOut);
        audioOut->setVolume(1);

        // BGM(索引2) 设置无限循环
        if (i == 2) player->setLoops(QMediaPlayer::Infinite);

        m_audioOutputs.push_back(audioOut);
        m_players.push_back(player);
        m_mediaLists.push_back(QVector<QUrl>());
    }

    // 加载音频配置
    initPlayList();
}

BGMControl::~BGMControl()
{
    qDeleteAll(m_players);
    qDeleteAll(m_audioOutputs);
    m_players.clear();
    m_audioOutputs.clear();
    m_mediaLists.clear();
}

void BGMControl::initPlayList()
{
    QStringList typeList = {"Man", "Woman", "BGM", "Other", "Ending"};

    // 读json配置文件
    QFile file(":/conf/playList.json");
    if (!file.open(QFile::ReadOnly)) return;

    // 解析从文件中读出的json数据
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonObject root = doc.object();

    // 加载每组音频路径
    for (int i = 0, size = (int)typeList.size(); i < size; ++i)
    {
        QJsonArray array = root[typeList[i]].toArray();
        for (const auto& item : array) m_mediaLists[i].append(QUrl(item.toString()));
    }
}

void BGMControl::startBGM(float volume)
{
    m_audioOutputs[2]->setVolume(volume);
    m_players[2]->play();
}

void BGMControl::stopBGM()
{
    m_players[2]->stop();
}

void BGMControl::playerRobLordMusic(int point, RoleSex sex, bool isFirst)
{
    int index = (sex == RoleSex::Man) ? 0 : 1;
    int audioIdx = 0;

    if (isFirst && point > 0)
        audioIdx = (int)CardType::Order;
    else if (point == 0)
        audioIdx = isFirst ? (int)CardType::NoOrder : (int)CardType::NoRob;
    else if (point == 2)
        audioIdx = (int)CardType::Rob1;
    else if (point == 3)
        audioIdx = (int)CardType::Rob2;

    // Qt6：直接设置音源播放
    m_players[index]->setSource(m_mediaLists[index][audioIdx]);
    m_players[index]->play();
}

void BGMControl::playCardMusic(Cards cards, bool isFirst, RoleSex sex)
{
    int index = (sex == RoleSex::Man) ? 0 : 1;
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getHandType();
    Card::CardPoint pt = Card::CardPoint::Card_Begin;
    int audioIdx = 0;

    if (type == PlayHand::HandType::Hand_Single || type == PlayHand::HandType::Hand_Pair || type == PlayHand::HandType::Hand_Triple)
    {
        pt = cards.takeRandomCard().getCardPoint();
    }

    switch (type)
    {
    case PlayHand::HandType::Hand_Single: audioIdx = (int)pt - 1; break;
    case PlayHand::HandType::Hand_Pair: audioIdx = (int)pt - 1 + 15; break;
    case PlayHand::HandType::Hand_Triple: audioIdx = (int)pt - 1 + 28; break;
    case PlayHand::HandType::Hand_Triple_Single: audioIdx = (int)CardType::ThreeBindOne; break;
    case PlayHand::HandType::Hand_Triple_Pair: audioIdx = (int)CardType::ThreeBindPair; break;
    case PlayHand::HandType::Hand_Plane:
    case PlayHand::HandType::Hand_Plane_Two_Single:
    case PlayHand::HandType::Hand_Plane_Two_Pair: audioIdx = (int)CardType::Plane; break;
    case PlayHand::HandType::Hand_Seq_Pair: audioIdx = (int)CardType::SequencePair; break;
    case PlayHand::HandType::Hand_Seq_Single: audioIdx = (int)CardType::Sequence; break;
    case PlayHand::HandType::Hand_Bomb: audioIdx = (int)CardType::Bomb; break;
    case PlayHand::HandType::Hand_Bomb_Jokers: audioIdx = (int)CardType::JokerBomb; break;
    case PlayHand::HandType::Hand_Bomb_Pair:
    case PlayHand::HandType::Hand_Bomb_Two_Single:
    case PlayHand::HandType::Hand_Bomb_Jokers_Pair:
    case PlayHand::HandType::Hand_Bomb_Jokers_Two_Single: audioIdx = (int)CardType::FourBindTwo; break;
    default: break;
    }

    // 随机大你/压死
    if (!isFirst && audioIdx >= (int)CardType::Plane && audioIdx <= (int)CardType::FourBindTwo)
    {
        audioIdx = (int)CardType::MoreBiger1 + QRandomGenerator::global()->bounded(2);
    }

    // 播放音效
    m_players[index]->setSource(m_mediaLists[index][audioIdx]);
    m_players[index]->play();

    // 联动辅助音效
    if (audioIdx == (int)CardType::Bomb || audioIdx == (int)CardType::JokerBomb)
        playAssistMusic(AssistMusic::BombVoice);
    if (audioIdx == (int)CardType::Plane)
        playAssistMusic(AssistMusic::PlaneVoice);
}

void BGMControl::playLastMusic(CardType type, RoleSex sex)
{
    int index = (sex == RoleSex::Man) ? 0 : 1;
    int audioIdx = (int)type;
    auto player = m_players[index];

    if (player->playbackState() == QMediaPlayer::StoppedState)
    {
        player->setSource(m_mediaLists[index][audioIdx]);
        player->play();
    }
    else
    {
        QTimer::singleShot(1500, this, [=]()
                           {
            player->setSource(m_mediaLists[index][audioIdx]);
            player->play(); });
    }
}

void BGMControl::playPassMusic(RoleSex sex)
{
    int index = (sex == RoleSex::Man) ? 0 : 1;
    int rand = QRandomGenerator::global()->bounded(4);
    int audioIdx = (int)CardType::Pass1 + rand;

    m_players[index]->setSource(m_mediaLists[index][audioIdx]);
    m_players[index]->play();
}

void BGMControl::playAssistMusic(AssistMusic type)
{
    int index = 3;
    int audioIdx = (int)type;
    auto player = m_players[index];

    // 发牌音效循环
    if (type == AssistMusic::Dispatch)
    {
        player->setLoops(QMediaPlayer::Infinite);
    }
    else
    {
        player->setLoops(1); // 单次播放
    }

    player->setSource(m_mediaLists[index][audioIdx]);
    player->play();
}

void BGMControl::stopAssistMusic()
{
    m_players[3]->stop();
}

void BGMControl::playEndingMusic(bool isWin)
{
    int index = 4;
    int audioIdx = isWin ? 0 : 1;
    m_players[index]->setSource(m_mediaLists[index][audioIdx]);
    m_players[index]->play();
}
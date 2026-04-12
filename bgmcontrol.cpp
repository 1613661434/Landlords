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

void BGMControl::playerRobLordMusic(int point, bool isMan, bool isFirst)
{
    int index = isMan ? 0 : 1;
    int audioIdx = 0;

    if (isFirst && point > 0)
        audioIdx = (int)MusicType::Order;
    else if (point == 0)
        audioIdx = isFirst ? (int)MusicType::NoOrder : (int)MusicType::NoRob;
    else if (point == 2)
        audioIdx = (int)MusicType::Rob1;
    else if (point == 3)
        audioIdx = (int)MusicType::Rob2;

    // 设置音源播放
    m_players[index]->setSource(m_mediaLists[index][audioIdx]);
    m_players[index]->play();
}

void BGMControl::playCardMusic(Cards cards, bool isFirst, bool isMan)
{
    // 得到播放列表
    int index = isMan ? 0 : 1;

    // 取出牌型 然后进行判断
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getHandType();
    int audioIdx = 0;

    switch (type)
    {
    case PlayHand::HandType::Hand_Single: audioIdx = (int)cards.minPoint() - 1; break;
    case PlayHand::HandType::Hand_Pair: audioIdx = (int)cards.minPoint() + 14; break;
    case PlayHand::HandType::Hand_Triple: audioIdx = (int)cards.minPoint() + 27; break;
    case PlayHand::HandType::Hand_Triple_Single: audioIdx = (int)MusicType::ThreeBindOne; break;
    case PlayHand::HandType::Hand_Triple_Pair: audioIdx = (int)MusicType::ThreeBindPair; break;
    case PlayHand::HandType::Hand_Plane:
    case PlayHand::HandType::Hand_Plane_Two_Single:
    case PlayHand::HandType::Hand_Plane_Two_Pair: audioIdx = (int)MusicType::Plane; break;
    case PlayHand::HandType::Hand_Seq_Pair: audioIdx = (int)MusicType::SequencePair; break;
    case PlayHand::HandType::Hand_Seq_Single: audioIdx = (int)MusicType::Sequence; break;
    case PlayHand::HandType::Hand_Bomb: audioIdx = (int)MusicType::Bomb; break;
    case PlayHand::HandType::Hand_Bomb_Jokers: audioIdx = (int)MusicType::JokerBomb; break;
    case PlayHand::HandType::Hand_Bomb_Pair:
    case PlayHand::HandType::Hand_Bomb_Two_Single:
    case PlayHand::HandType::Hand_Bomb_Jokers_Pair:
    case PlayHand::HandType::Hand_Bomb_Jokers_Two_Single: audioIdx = (int)MusicType::FourBindTwo; break;
    default: break;
    }

    // 随机大你/管上
    if (!isFirst && audioIdx >= (int)MusicType::Plane && audioIdx <= (int)MusicType::FourBindTwo)
    {
        audioIdx = (int)MusicType::MoreBiger1 + QRandomGenerator::global()->bounded(2); // 没有包含压死，不做了
    }

    // 播放音效
    m_players[index]->setSource(m_mediaLists[index][audioIdx]);
    m_players[index]->play();

    // 联动辅助音效
    if (audioIdx == (int)MusicType::Bomb || audioIdx == (int)MusicType::JokerBomb)
        playAssistMusic(AssistMusicType::BombVoice);
    else if (audioIdx == (int)MusicType::Plane)
        playAssistMusic(AssistMusicType::PlaneVoice);
}

void BGMControl::playLastMusic(MusicType type, bool isMan)
{
    int index = isMan ? 0 : 1;
    auto player = m_players[index];

    if (player->playbackState() == QMediaPlayer::StoppedState)
    {
        player->setSource(m_mediaLists[index][(int)type]);
        player->play();
    }
    else
    {
        QTimer::singleShot(1500, this, [=]()
                           {
            player->setSource(m_mediaLists[index][(int)type]);
            player->play(); });
    }
}

void BGMControl::playPassMusic(bool isMan)
{
    int index = isMan ? 0 : 1;
    int audioIdx = (int)MusicType::Pass1 + QRandomGenerator::global()->bounded(4);

    m_players[index]->setSource(m_mediaLists[index][audioIdx]);
    m_players[index]->play();
}

void BGMControl::playAssistMusic(AssistMusicType type)
{
    auto player = m_players[3];

    // 发牌音效循环
    if (type == AssistMusicType::Dispatch)
        player->setLoops(QMediaPlayer::Infinite);
    else
        player->setLoops(1); // 单次播放

    player->setSource(m_mediaLists[3][(int)type]);
    player->play();
}

void BGMControl::stopAssistMusic()
{
    m_players[3]->stop();
}

void BGMControl::playEndingMusic(bool isWin)
{
    int audioIdx = isWin ? 0 : 1;
    m_players[4]->setSource(m_mediaLists[4][audioIdx]);
    m_players[4]->play();
}
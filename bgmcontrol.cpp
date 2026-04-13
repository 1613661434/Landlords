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
    // 初始化5组音频单元
    for (int i = 0; i < 5; ++i)
    {
        // 1. 创建音频组件
        QAudioOutput* output = new QAudioOutput(this);
        QMediaPlayer* player = new QMediaPlayer(this);
        ol::MediaPlaylist* playlist = new ol::MediaPlaylist(this);

        // 2. 绑定播放器与播放列表（自动切歌/循环）
        player->setAudioOutput(output);
        playlist->setPlayer(player);
        output->setVolume(1.0f);

        // 3. 设置播放模式
        if (i == 2) // BGM：列表循环
            playlist->setPlaybackMode(ol::MediaPlaylist::PlaybackMode::SequentialLoop);
        else // 默认：单次播放
            playlist->setPlaybackMode(ol::MediaPlaylist::PlaybackMode::CurrentItemOnce);

        // 4. 存入容器
        m_outputs.push_back(output);
        m_players.push_back(player);
        m_playlists.push_back(playlist);
    }

    initPlayList();
}

BGMControl::~BGMControl()
{
    qDeleteAll(m_playlists);
    qDeleteAll(m_players);
    qDeleteAll(m_outputs);
    m_playlists.clear();
    m_players.clear();
    m_outputs.clear();
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

    // 加载音频到播放列表
    for (int i = 0, size = (int)typeList.size(); i < size; ++i)
    {
        QJsonArray array = root[typeList[i]].toArray();
        for (const auto& item : array) m_playlists[i]->addMedia(QUrl(item.toString()));
    }
}

void BGMControl::startBGM(float volume)
{
    m_outputs[2]->setVolume(volume);
    m_playlists[2]->setCurrentIndex(0);
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
    m_playlists[index]->setCurrentIndex(audioIdx);
    m_players[index]->play();
}

void BGMControl::playCardMusic(Cards cards, bool isFirst, bool isMan)
{
    // 得到播放列表
    int index = isMan ? 0 : 1;

    // 取出牌型，然后进行判断
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
    m_playlists[index]->setCurrentIndex(audioIdx);
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
        m_playlists[index]->setCurrentIndex((int)type);
        player->play();
    }
    else
    {
        QTimer::singleShot(1500, this, [=]()
                           {
            m_playlists[index]->setCurrentIndex((int)type);
            player->play(); });
    }
}

void BGMControl::playPassMusic(bool isMan)
{
    int index = isMan ? 0 : 1;
    int audioIdx = (int)MusicType::Pass1 + QRandomGenerator::global()->bounded(4);

    m_playlists[index]->setCurrentIndex(audioIdx);
    m_players[index]->play();
}

void BGMControl::playAssistMusic(AssistMusicType type)
{
    auto playlist = m_playlists[3];
    auto player = m_players[3];

    // 发牌：单曲循环 | 其他：单次播放
    if (type == AssistMusicType::Dispatch)
        playlist->setPlaybackMode(ol::MediaPlaylist::PlaybackMode::CurrentItemLoop);
    else
        playlist->setPlaybackMode(ol::MediaPlaylist::PlaybackMode::CurrentItemOnce);

    playlist->setCurrentIndex((int)type);
    player->play();
}

void BGMControl::stopAssistMusic()
{
    m_players[3]->stop();
}

void BGMControl::playEndingMusic(bool isWin)
{
    int audioIdx = isWin ? 0 : 1;
    m_playlists[4]->setCurrentIndex(audioIdx);
    m_players[4]->play();
}
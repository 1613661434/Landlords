#ifndef BGMCONTROL_H
#define BGMCONTROL_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVector>
#include <QUrl>
#include "cards.h"

class BGMControl : public QObject
{
    Q_OBJECT
public:
    // 性别枚举
    enum class RoleSex : char
    {
        Man,
        Woman
    };
    // 卡牌音效类型
    enum class CardType : char
    {
        // 单张牌
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace,
        Two,
        SmallJoker,
        BigJoker,
        // 对子
        Three_Double,
        Four_Double,
        Five_Double,
        Six_Double,
        Seven_Double,
        Eight_Double,
        Nine_Double,
        Ten_Double,
        Jack_Double,
        Queen_Double,
        King_Double,
        Ace_Double,
        Two_Double,
        // 三张
        Three_Triple,
        Four_Triple,
        Five_Triple,
        Six_Triple,
        Seven_Triple,
        Eight_Triple,
        Nine_Triple,
        Ten_Triple,
        Jack_Triple,
        Queen_Triple,
        King_Triple,
        Ace_Triple,
        Two_Triple,
        // 组合牌型
        Plane,
        SequencePair,
        ThreeBindOne,
        ThreeBindPair,
        Sequence,
        FourBindTwo,
        FourBind2Pair,
        Bomb,
        JokerBomb,
        // 过
        Pass1,
        Pass2,
        Pass3,
        Pass4,
        // 压制
        MoreBiger1,
        MoreBiger2,
        Biggest,
        // 抢地主
        NoOrder,
        NoRob,
        Order,
        Rob1,
        Rob2,
        // 剩余牌
        Last1,
        Last2
    };
    // 辅助音效
    enum class AssistMusic : char
    {
        Dispatch,
        SelectCard,
        PlaneVoice,
        BombVoice,
        Alert
    };

private:
    // 5组播放器：0男 1女 2BGM 3辅助音效 4结束音乐
    QVector<QMediaPlayer*> m_players;      // 播放器
    QVector<QAudioOutput*> m_audioOutputs; // 音频输出
    QVector<QVector<QUrl>> m_mediaLists;   // 存储每组音频路径

public:
    explicit BGMControl(QObject* parent = nullptr);
    ~BGMControl() override; // 析构释放内存

    // 初始化音频列表
    void initPlayList();

    // 1. 背景音乐
    void startBGM(float volume);
    void stopBGM();

    // 2. 抢地主音效
    void playerRobLordMusic(int point, RoleSex sex, bool isFirst);

    // 3. 出牌音效
    void playCardMusic(Cards cards, bool isFirst, RoleSex sex);
    void playLastMusic(CardType type, RoleSex sex);

    // 4. 不出牌音效
    void playPassMusic(RoleSex sex);

    // 5. 辅助音效
    void playAssistMusic(AssistMusic type);
    void stopAssistMusic();

    // 6. 结束音效
    void playEndingMusic(bool isWin);
};

#endif // BGMCONTROL_H
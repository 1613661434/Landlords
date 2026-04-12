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
        // 两张牌
        Three_Double,
        Foue_Double,
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
        Tow_Double,
        // 三张牌
        Three_Triple,
        Foue_Triple,
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
        Tow_Triple,
        // 其他组合
        Plane,         // 飞机
        SequencePair,  // 连对
        ThreeBindOne,  // 三带一
        ThreeBindPair, // 三带一对
        Sequence,      // 顺子
        FourBindTwo,   // 四带二(单张)
        FourBind2Pair, // 四带两对
        Bomb,          // 炸弹
        JokerBomb,     // 王炸
        Pass1,         // 过
        Pass2,
        Pass3,
        Pass4,
        MoreBiger1, // 大你
        MoreBiger2,
        Biggest, // 压死
                 // 抢地主
        NoOrder, // 不叫
        NoRob,   // 不抢
        Order,   // 叫地主
        Rob1,    // 抢地主
        Rob2,
        Last1, // 只剩1张牌
        Last2  // 只剩2张牌
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
    void playerRobLordMusic(int point, bool isMan, bool isFirst);

    // 3. 出牌音效
    void playCardMusic(Cards cards, bool isFirst, bool isMan);
    void playLastMusic(CardType type, bool isMan);

    // 4. 不出牌音效
    void playPassMusic(bool isMan);

    // 5. 辅助音效
    void playAssistMusic(AssistMusic type);
    void stopAssistMusic();

    // 6. 结束音效
    void playEndingMusic(bool isWin);
};

#endif // BGMCONTROL_H
#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QLabel>

namespace Ui
{
    class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT
public:
    enum class FontColor : char
    {
        Black,
        White,
        Red,
        Blue,
        Green
    };

private:
    Ui::ScorePanel* ui;
    QVector<QLabel*> m_list;

public:
    explicit ScorePanel(QWidget* parent = nullptr);
    ~ScorePanel();

    // 设置玩家的得分
    void setScores(int left, int right, int user);
    // 设置字体大小
    void setMyFontSize(int size);
    // 设置字体的颜色
    void setMyFontColor(FontColor color);
};

#endif // SCOREPANEL_H

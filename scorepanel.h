#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

namespace Ui
{
    class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT

private:
    Ui::ScorePanel* ui;

public:
    explicit ScorePanel(QWidget* parent = nullptr);
    ~ScorePanel();

    // 设置玩家的得分
    void setScores(int left, int right, int user);
};

#endif // SCOREPANEL_H

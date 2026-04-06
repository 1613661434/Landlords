#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "scorepanel.h"

class EndingPanel : public QWidget
{
    Q_OBJECT
private:
    QPixmap m_image;
    QLabel* m_title;
    ScorePanel* m_score;
    QPushButton* m_continue;

public:
    explicit EndingPanel(bool isLord, bool isWin, QWidget* parent = nullptr);

    void setPlayerScore(int left, int right, int me);

protected:
    void paintEvent(QPaintEvent* ev) override;

signals:
    void continueGame();
};

#endif // ENDINGPANEL_H

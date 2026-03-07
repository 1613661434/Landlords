#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT
private:
    QPixmap m_bkImage;

public:
    GamePanel(QWidget* parent = nullptr);
    ~GamePanel();

protected:
    // 绘图
    void paintEvent(QPaintEvent* ev) override;

private:
    Ui::GamePanel* ui;
};
#endif // GAMEPANEL_H

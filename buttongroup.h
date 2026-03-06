#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui
{
    class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonGroup(QWidget* parent = nullptr);
    ~ButtonGroup();

    // 初始化按钮
    void initButtons();

private:
    Ui::ButtonGroup* ui;
};

#endif // BUTTONGROUP_H
